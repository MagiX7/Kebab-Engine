#include "Application.h"

#include "Renderer3D.h"
#include "MainScene.h"

#include "Window.h"
#include "Camera3D.h"
#include "Input.h"
#include "Editor.h"

#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"

#include "ResourceManager.h"

#include "Material.h"
#include "Shader.h"

#include "QdTree.h"
#include "KbPyramid.h"

#include "Math/float4x4.h"

#include "SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include <queue>

#include "mmgr/mmgr.h"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

Renderer3D::Renderer3D(bool startEnabled) : Module(true)
{
	name = "renderer";
}

// Destructor
Renderer3D::~Renderer3D()
{}

// Called before render is available
bool Renderer3D::Init(JSON_Object* root)
{
	LOG_CONSOLE("Creating 3D Renderer context");
	
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG_CONSOLE("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG_CONSOLE("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glLoadMatrixf(app->camera->GetCurrentCamera()->frustum.ProjectionMatrix().Transposed().ptr());

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		dirLight = new DirectionalLight();

		lights[0].ref = GL_LIGHT0;
		//lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].ambient.Set(dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z, 1.0f);
		//lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].diffuse.Set(dirLight->diffuse.x, dirLight->diffuse.x, dirLight->diffuse.x, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		lights[0].Active(true);

		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		LOG_CONSOLE("OpenGL initialization correct. Version %s", glGetString(GL_VERSION));

		// GLEW initialization
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			LOG_CONSOLE("Error loading GLEW: %s", glewGetErrorString(err));
		}
		else LOG_CONSOLE("GLEW initialization correct. Version %s", glewGetString(GLEW_VERSION));

		// Theorically for anti-aliasing (MSAA)
		//glEnable(GL_MULTISAMPLE);
	}

	Load(root);

	SetDepth();
	SetCullFace();
	SetLighting();
	SetColorMaterial();
	SetTexture2D();
	SetWireframe();
	SetBlending();

	drawAABB = false;
	drawGrid = true;

	int w, h;
	app->window->GetSize(w, h);
	OnResize(w, h);

	FrameBufferProperties props;
	props.width = w;
	props.height = h;
	editorFbo = new FrameBuffer(props);
	sceneFbo = new FrameBuffer(props);

	defaultShader = new Shader("Assets/Resources/Shaders/default.shader");
	shaders.push_back(defaultShader);

	defaultMaterial = new Material();
	defaultMaterial->SetShader(defaultShader);

	gammaCorrection = true;
	gammaCorrectionAmount = 1.0f;

	return ret;
}

bool Renderer3D::Start()
{
	goDirLight = new GameObject("Directional Light");
	ComponentLight* lightComp = new ComponentLight();
	lightComp->SetLight(dirLight);
	goDirLight->AddComponent(lightComp);
	lightComp->SetParent(goDirLight);
	app->scene->AddGameObject(goDirLight);

	ComponentTransform* tr = (ComponentTransform*)goDirLight->GetComponent(ComponentType::TRANSFORM);
	tr->SetRotation({ 173, 0, -63 });
	tr->SetTranslation({ -8, 17, -0.45 });

	//GameObject* go = MeshLoader::GetInstance()->LoadKbGeometry(KbGeometryType::PYRAMID, true);
	//goDirLight->AddChild(go);
	//go->SetParent(goDirLight);
	//ComponentTransform* childTr = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);
	//childTr->SetGlobalMatrix(tr->GetGlobalMatrix());

	// TODO: It draws in game scene, should we create another list for go's
	// that only are for dev purposes?
	// Draw it with UI when it is done?
	//Submit(goDirLight);

	// Set the water shader to the plane

	lights[0].position.x = tr->GetTranslation().x;
	lights[0].position.y = tr->GetTranslation().y;
	lights[0].position.z = tr->GetTranslation().z;

	return true;
}

// PreUpdate: clear buffer
bool Renderer3D::PreUpdate(float dt)
{
	glClearColor(0.05f, 0.05f, 0.05f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		wireframe = !wireframe;
		SetWireframe();
	}

	// Light 0 on cam pos
	lights[0].SetPos(app->camera->position.x, app->camera->position.y, app->camera->position.z);

	if (dirLight)
	{
		lights[0].ambient.Set(dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z, 1.0f);
		lights[0].diffuse.Set(dirLight->diffuse.x, dirLight->diffuse.x, dirLight->diffuse.x, 1.0f);
	}
	else
	{
		lights[0].ambient.Set(0,0,0, 1.0f);
		lights[0].diffuse.Set(0,0,0, 1.0f);
	}

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return true;
}

// Draw present buffer to screen
bool Renderer3D::Draw(float dt)
{
	app->editor->OnImGuiRender(dt, editorFbo, sceneFbo);

	sceneFbo->Bind();

	if (app->camera->gameCam)
		PushCamera(app->camera->gameCam);

	glClearColor(0.1f, 0.1f, 0.1f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawGrid)
		DrawGrid();

	if (app->editor->debugQT)
		app->scene->rootQT->DrawTree();

	glPopMatrix();
	glPopMatrix();

	if (app->camera->gameCam)
	{
		DoRender(true);
	}
	sceneFbo->Unbind();


	editorFbo->Bind();
	PushCamera(app->camera->editorCam);
	glClearColor(0.1f, 0.1f, 0.1f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawGrid)
		DrawGrid();

	if (app->editor->debugQT)
		app->scene->rootQT->DrawTree();

	app->camera->DrawPickingRay();

	if (ComponentCamera* c = app->camera->gameCam)
		c->DrawFrustum();
	
	glPopMatrix();
	glPopMatrix();

	DoRender(false);
	editorFbo->Unbind();

	SDL_GL_SwapWindow(app->window->window);
	return true;
}

// Called before quitting
bool Renderer3D::CleanUp()
{
	LOG_CONSOLE("Destroying 3D Renderer");

	gameObjects.clear();

	meshes.clear();

	delete(editorFbo);
	delete(sceneFbo);
	delete(defaultMaterial);
	//delete(defaultShader);

	for (auto& s : shaders)
	{
		delete s;
		s = 0;
	}
	shaders.clear();

	delete dirLight;
	dirLight = 0;

	SDL_GL_DeleteContext(context);

	return true;
}

void Renderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	float ratio = (float)width / (float)height;
	app->camera->SetRatio(ratio);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(app->camera->GetCurrentCamera()->frustum.ProjectionMatrix().Transposed().ptr());

	glMatrixMode(GL_MODELVIEW);
	float4x4 mat = app->camera->GetCurrentCamera()->frustum.ViewMatrix();
	glLoadMatrixf(mat.Transposed().ptr());

}

void Renderer3D::SetDepth()
{
	depth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	LOG_CONSOLE("-- GL_DEPTH_TEST -- set to %d", depth);
}

void Renderer3D::SetCullFace()
{
	cullFace ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
	LOG_CONSOLE("-- GL_CULL_FACE -- set to %d", cullFace);
}

void Renderer3D::SetLighting()
{
	lighting ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	LOG_CONSOLE("-- GL_LIGHTING -- set to %d", lighting);
}

void Renderer3D::SetColorMaterial()
{
	colorMaterial ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
	LOG_CONSOLE("-- GL_COLOR_MATERIAL -- set to %d", colorMaterial);
}

void Renderer3D::SetTexture2D()
{
	texture2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	LOG_CONSOLE("-- GL_TEXTURE_2D -- set to %d", texture2D);
}

void Renderer3D::SetWireframe()
{
	wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	LOG_CONSOLE("-- WIREFRAME -- set to %d", wireframe);
}

void Renderer3D::SetBlending()
{
	blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	LOG_CONSOLE("-- BLEND -- set to %d", blend);
}

void Renderer3D::Save(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* renObj = json_object_get_object(root, name.c_str());

	json_object_set_boolean(renObj, "depth", depth);
	json_object_set_boolean(renObj, "cullface", cullFace);
	json_object_set_boolean(renObj, "lighting", lighting);
	json_object_set_boolean(renObj, "color material", colorMaterial);
	json_object_set_boolean(renObj, "texture2D", texture2D);
	json_object_set_boolean(renObj, "wireframe", wireframe);
}

void Renderer3D::Load(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_object_get_value(root, name.c_str()));
	JSON_Object* renObj = json_object_get_object(root, name.c_str());

	depth = json_object_get_boolean(renObj, "depth");
	cullFace = json_object_get_boolean(renObj, "cullface");
	lighting = json_object_get_boolean(renObj, "lighting");
	colorMaterial = json_object_get_boolean(renObj, "color material");
	texture2D = json_object_get_boolean(renObj, "texture2D");
	wireframe = json_object_get_boolean(renObj, "wireframe");
	blend = json_object_get_boolean(renObj, "blend");
}

void Renderer3D::Submit(GameObject* go)
{
	std::queue<GameObject*> q;
	q.push(go);

	while(!q.empty())
	{
		auto& curr = q.front();
		q.pop();

		if (curr->isStatic)
		{
			app->scene->rootQT->Insert(curr);
			app->scene->rootQT->Recalculate();
		}

		if (curr->GetComponent(ComponentType::MESH))
			if (std::find(gameObjects.begin(), gameObjects.end(), curr) == gameObjects.end())
				gameObjects.push_back(curr);

		for (auto& child : curr->GetChilds())
			q.push(child);
	}
}

void Renderer3D::EraseGameObject(GameObject* go)
{
	if (go->GetChilds().size() > 0)
	{
		for (const auto& child : go->GetChilds())
		{
			ComponentMesh* mesh = (ComponentMesh*)child->GetComponent(ComponentType::MESH);
			if(mesh)
				EraseGameObject(child);
		}
	}

	if (go->GetComponent(ComponentType::MESH))
	{
		if (gameObjects.size() > 0)
		{
			std::vector<GameObject*>::iterator it = gameObjects.begin();
			while (*it != go && it != gameObjects.end())
				++it;

			gameObjects.erase(it);
			gameObjects.shrink_to_fit();
		}
	}
}

void Renderer3D::EraseAllGameObjects()
{	
	gameObjects.clear();
}

void Renderer3D::DrawGrid()
{
	glLineWidth(1.5f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	float d = 200.0f;

	glColor4f(1.0f, 1.0f, 1.0f, 0.65f);
	glLineWidth(1.0f);

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Renderer3D::DrawAABB(AABB& aabb)
{
	GLdouble min[3] = { aabb.MinX(), aabb.MinY(), aabb.MinZ() };
	GLdouble max[3] = { aabb.MaxX(), aabb.MaxY(), aabb.MaxZ() };

	glBegin(GL_LINE_LOOP);
	glVertex3dv(&min[0]);
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3d(min[0], max[1], min[2]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], min[1], max[2]);
	glVertex3dv(&max[0]);
	glVertex3d(min[0], max[1], max[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3dv(&min[0]);
	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], min[1], max[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3dv(&max[0]);
	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(min[0], max[1], max[2]);
	glEnd();
}
Material* Renderer3D::GetDefaultMaterial()
{
	return defaultMaterial;
}

Shader* Renderer3D::GetDefaultShader()
{
	return defaultShader;
}

Shader* Renderer3D::AddShader(const std::string& path)
{
	Shader* shader = new Shader(path);
	shaders.push_back(shader);

	if (path.find("default"))
	{
		defaultShader = shader;
	}

	return shader;
}

void Renderer3D::AddMaterial(Material* material)
{
	if (material)
	{
		for (auto& go : gameObjects)
		{
			ComponentMaterial* mat = (ComponentMaterial*)go->GetComponent(ComponentType::MATERIAL);
			if(!mat->GetMaterial())
				mat->SetMaterial(material);
		}
	}

	materials.push_back(material);
}

void Renderer3D::AddPointLight(PointLight* pl)
{
	if(pLights.size() < MAX_POINT_LIGHTS)
		pLights.push_back(pl);
	else
		LOG_CONSOLE("Max number of Point Lights reached");
}

void Renderer3D::AddSpotLight(SpotLight* sl)
{
	if (spotLights.size() < MAX_SPOT_LIGHTS)
		spotLights.push_back(sl);
	else
		LOG_CONSOLE("Max number of Spot Lights reached");
}

void Renderer3D::DeletePointLight(PointLight* pl)
{
	std::vector<PointLight*>::iterator it = pLights.begin();

	for (; it != pLights.end(); ++it)
	{
		if ((*it) == pl)
		{
			delete pl;
			pl = 0;
			*it = 0;
			pLights.erase(it);
			
			break;
		}
	}

	/*for (auto& l : pLights)
	{
		if (l == pl)
		{
			pLights.erase();
			delete l;
			l = 0;

			break;
		}
	}*/
}

void Renderer3D::DoRender(bool gameScene)
{
	for (const auto& go : gameObjects)
	{
		ComponentMaterial* mat = (ComponentMaterial*)go->GetComponent(ComponentType::MATERIAL);
		ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);

		if (gameScene)
		{
			if (mesh && mat && !app->editor->frustumCulling)
			{
				mesh->Draw(mat, app->camera->gameCam);
				if (drawAABB)
					DrawAABB(*go->GetGlobalAABB());
			}
			else if (mesh && mat && go->insideFrustum && app->editor->frustumCulling)
			{
				mesh->Draw(mat, app->camera->gameCam);
				if (drawAABB)
					DrawAABB(*go->GetGlobalAABB());
			}
		}
		else
		{
			if (mesh && mat && !app->editor->frustumCulling)
			{
				mesh->Draw(mat, app->camera->editorCam);
				if (drawAABB)
					DrawAABB(*go->GetGlobalAABB());
			}
			else if (mesh && mat && go->insideFrustum && app->editor->frustumCulling)
			{
				mesh->Draw(mat, app->camera->editorCam);
				if (drawAABB)
					DrawAABB(*go->GetGlobalAABB());
			}

		}
	}

	// TO DRAW PARENTS AABB WITH NO MESH (IT WORKS STRANGE)
	/*std::queue<GameObject*> que;
	que.push(app->scene->GetRoot());

	while (!que.empty())
	{
		GameObject* curr = que.front();
		que.pop();

		curr->SetGlobalAABB(curr);

		if (drawAABB && curr->GetGlobalAABB()->Volume() != 0)
			DrawAABB(*curr->GetGlobalAABB());

		for (GameObject* child : curr->GetChilds())
			que.push(child);
	}*/
}

void Renderer3D::PushCamera(ComponentCamera* cam)
{
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cam->frustum.ProjectionMatrix().Transposed().ptr());

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	float4x4 mat = cam->frustum.ViewMatrix();
	glLoadMatrixf(mat.Transposed().ptr());
}
