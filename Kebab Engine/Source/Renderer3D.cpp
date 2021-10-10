#include "Application.h"
#include "Renderer3D.h"
#include "SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "mmgr/mmgr.h"

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
	LOG("Creating 3D Renderer context");
	
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		LOG("OpenGL initialization correct. Version %s", glGetString(GL_VERSION));

		// GLEW initialization
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			LOG("Error loading GLEW: %s", glewGetErrorString(err));
		}
		else LOG("GLEW initialization correct. Version %s", glewGetString(GLEW_VERSION));
	}

	Load(root);

	if (depth) SetDepth(depth);
	if (cullFace) SetCullFace(cullFace);
	lights[0].Active(true);
	if (lighting) SetLighting(lighting);
	if (colorMaterial) SetColorMaterial(colorMaterial);
	if (texture2D) SetTexture2D(texture2D);
	if (wireframe) SetWireframe(wireframe);

	int w, h;
	app->window->GetWindowSize(w, h);
	OnResize(w, h);


	vertexArray = new VertexArray();
	indexBuffer = new IndexBuffer();
	vertexBuffer = new VertexBuffer();

	return ret;
}

// PreUpdate: clear buffer
bool Renderer3D::PreUpdate(float dt)
{
	glClearColor(0.05f, 0.05f, 0.05f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();

	//glMatrixMode(GL_MODELVIEW);
	
	glLoadMatrixf(app->camera->GetViewMatrix());
	

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		wireframe = !wireframe;
		wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// light 0 on cam pos
	lights[0].SetPos(app->camera->position.x, app->camera->position.y, app->camera->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return true;
}

// Draw present buffer to screen
bool Renderer3D::Draw(float dt)
{
	/*for (const auto& g : geometries)
		g->Draw();*/

	SDL_GL_SwapWindow(app->window->window);
	return true;
}

// Called before quitting
bool Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	geometries.clear();
	/*delete vertexArray;
	delete vertexBuffer;
	delete indexBuffer;*/

	RELEASE(vertexArray);
	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);

	SDL_GL_DeleteContext(context);

	return true;
}

void Renderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	app->window->SetWindowSize(width, height);
}

void Renderer3D::SetDepth(bool value)
{
	depth = value;
	value ? glDisable(GL_DEPTH_TEST) : glEnable(GL_DEPTH_TEST);
	LOG("-- GL_DEPTH_TEST -- set to %d", value);
}

void Renderer3D::SetCullFace(bool value)
{
	cullFace = value;
	value ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
	LOG("-- GL_CULL_FACE -- set to %d", value);
}

void Renderer3D::SetLighting(bool value)
{
	lighting = value;
	value ? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);
	LOG("-- GL_LIGHTING -- set to %d", value);
}

void Renderer3D::SetColorMaterial(bool value)
{
	colorMaterial = value;
	value ? glDisable(GL_COLOR_MATERIAL) : glEnable(GL_COLOR_MATERIAL);
	LOG("-- GL_COLOR_MATERIAL -- set to %d", value);
}

void Renderer3D::SetTexture2D(bool value)
{
	texture2D = value;
	value ? glDisable(GL_TEXTURE_2D) : glEnable(GL_TEXTURE_2D);
	LOG("-- GL_TEXTURE_2D -- set to %d", value);
}

void Renderer3D::SetWireframe(bool value)
{
	wireframe = value;
	wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* renObj = json_object_get_object(root, name.c_str());

	depth = json_object_get_boolean(renObj, "depth");
	cullFace = json_object_get_boolean(renObj, "cullface");
	lighting = json_object_get_boolean(renObj, "lighting");
	colorMaterial = json_object_get_boolean(renObj, "color material");
	texture2D = json_object_get_boolean(renObj, "texture2D");
	wireframe = json_object_get_boolean(renObj, "wireframe");
}

void Renderer3D::Submit(KebabGeometry* geometry)
{
	geometries.push_back(geometry);
	numVertices += geometry->verticesCount;
	numIndices += geometry->indicesCount;
	vertexBuffer->AddData(geometry->vertices, sizeof(geometry->vertices) * geometry->verticesCount);
	indexBuffer->AddData(geometry->indices, geometry->indicesCount);

	vertexArray->AddVertexBuffer(*vertexBuffer);
	vertexArray->SetIndexBuffer(*indexBuffer);

	// TODO: Almacenar todo, inicializar buffers vacios y antes de drawear, hacer un AddData y luego
}

void Renderer3D::Submit(std::vector<KebabGeometry*> geos)
{
	//geometries.insert(geometries.end(), geos.begin(), geos.end());
	for (const auto& g : geos)
		Submit(g);
}

void Renderer3D::DoDraw()
{
	//frameBuffer->Bind();
	glClearColor(0.05f, 0.05f, 0.05f, 1);


	/*vertexArray->AddVertexBuffer(*vertexBuffer);
	vertexArray->SetIndexBuffer(*indexBuffer);*/

	vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	vertexArray->Unbind();

	//frameBuffer->Unbind();
}