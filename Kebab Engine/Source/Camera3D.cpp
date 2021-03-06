#include "Application.h"
#include "Camera3D.h"

#include "Input.h"
#include "Editor.h"
#include "MainScene.h"
#include "Renderer3D.h"

#include "PanelHierarchy.h"
#include "PanelViewport.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

#include "QdTree.h"

#include <queue>

#include "mmgr/mmgr.h"

Camera3D::Camera3D(bool startEnabled) : Module(startEnabled)
{
	name = "camera3D";

	position = vec(0.0f, 0.0f, 0.0f);
	reference = vec(0.0f, 0.0f, 0.0f);

	editorCam = new ComponentCamera(nullptr, CameraType::EDITOR);
	currentCam = editorCam;

	focusing = false;
	orbiting = false;

	editorCam->SetPosition(position);
	editorCam->Look(reference);

	editorCam->SetFarPlane(200);
}

Camera3D::~Camera3D()
{}

bool Camera3D::Init(JSON_Object* root)
{
	Load(root);

	return true;
}

// -----------------------------------------------------------------
bool Camera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool Camera3D::CleanUp()
{
	LOG("Cleaning camera");

	//delete currentCam;
	//currentCam = nullptr;
	//if(editorCam) delete editorCam;
	//if(gameCam) delete gameCam;

	delete editorCam;
	editorCam = nullptr;
	gameCam = nullptr;
	currentCam = nullptr;

	return true;
}

// -----------------------------------------------------------------
bool Camera3D::Update(float dt)
{	
	float3 newPos(0, 0, 0);
	float speed = 20.0f * dt;
  
	float dx = -app->input->GetMouseXMotion();
	float dy = -app->input->GetMouseYMotion();

	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 40.0f * dt;

	if (app->editor->viewportPanel->IsHovered())
	{
		if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			// Movement of camera ====================================================================================
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += editorCam->frustum.Front() * speed;
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= editorCam->frustum.Front() * speed;

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= editorCam->frustum.WorldRight() * speed;
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += editorCam->frustum.WorldRight() * speed;

			if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= float3::unitY * speed * 0.5f;
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += float3::unitY * speed * 0.5f;

			position += newPos;

			editorCam->SetPosition(position);

			// Rotation of camera ================================================================================
			math::Quat rotationX = math::Quat::RotateAxisAngle(float3::unitY, dx * DEGTORAD);
			math::Quat rotationY = math::Quat::RotateAxisAngle(editorCam->frustum.WorldRight(), dy * DEGTORAD);
			math::Quat finalRotation = rotationX * rotationY;

			editorCam->frustum.Transform(finalRotation);
		}

		// Zoom ===============================================================
		float3 zoom(0, 0, 0);
		float4 viewDim = app->editor->viewportPanel->GetDimensions();

		if (ImGui::GetMousePos().x > viewDim.x && ImGui::GetMousePos().x < viewDim.x + viewDim.z &&
			ImGui::GetMousePos().y > viewDim.y && ImGui::GetMousePos().y < viewDim.y + viewDim.w &&
			app->editor->viewportPanel->IsHovered())
		{
			if (app->input->GetMouseZ() < 0)
			{
				zoom -= editorCam->frustum.Front() * speed;
				position += zoom;
				editorCam->SetPosition(position);
			}
			if (app->input->GetMouseZ() > 0)
			{
				zoom += editorCam->frustum.Front() * speed;
				position += zoom;
				editorCam->SetPosition(position);
			}
		}

		// Focus
		if (app->editor->hierarchyPanel->currentGO)
		{
			GameObject* selectedGO = app->editor->hierarchyPanel->currentGO;
			ComponentTransform* compTransGO = (ComponentTransform*)selectedGO->GetComponent(ComponentType::TRANSFORM);

			AABB* boundBox = selectedGO->GetLocalAABB();

			if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) focusing = true;

			if (focusing == true)
			{
				if (boundBox->IsDegenerate())
				{
					float dist = 20;
					float3 dir = compTransGO->GetTranslation() - position;

					if (Distance(compTransGO->GetTranslation(), position) > dist + 0.5f)
						position += dir.Normalized();
					else if (Distance(compTransGO->GetTranslation(), position) < dist - 0.5f)
						position -= dir.Normalized();
					else
						focusing = false;

					reference = compTransGO->GetTranslation();
					editorCam->SetPosition(position);
					editorCam->Look(reference);
				}
				else
					CenterCameraToGO(boundBox);
			}

			if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT
				&& !ImGuizmo::IsUsing())
			{
				orbiting = true;
				OrbitGO(boundBox, dx, dy);
			}
			if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP
				&& !ImGuizmo::IsUsing())
				orbiting = false;
		}

		// Mouse Picking
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver()
			&& !orbiting && !focusing)
		{
			GameObject* picked = MousePickGameObject();
			app->editor->hierarchyPanel->SetCurrent(picked);
		}
	}

	// Frustum Culling
	if (gameCam != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = app->scene->GetGameObjects().begin(); it != app->scene->GetGameObjects().end(); ++it)
		{
			PropagateDrawInFrustumCulling((*it), gameCam);
		}
	}

	return true;
}

void Camera3D::LookAt( const float3& point)
{
	editorCam->Look(point);
	reference = point;
}

void Camera3D::MoveTo(const float3& pos)
{
	editorCam->SetPosition(pos);
	position = pos;
}

void Camera3D::SetPosLook(const float3& pos, const float3& pointLook)
{
	editorCam->Look(pointLook);
	reference = pointLook;
	editorCam->SetPosition(pos);
	position = pos;
}

void Camera3D::SetRatio(float ratio)
{
	editorCam->frustum.SetHorizontalFovAndAspectRatio(Atan(ratio * Tan(editorCam->frustum.VerticalFov() / 2)) * 2, ratio);
}

float* Camera3D::GetViewMatrix()
{
	return editorCam->GetViewMatrix().ptr();
}

float* Camera3D::GetProjectionMatrix()
{
	return editorCam->GetProjectionMatrix().ptr();
}

ComponentCamera* Camera3D::GetCurrentCamera()
{
	return currentCam;
}

void Camera3D::SetEditorCamera(ComponentCamera* cam)
{
	editorCam = cam;
}

void Camera3D::SetGameCamera(ComponentCamera* cam)
{
	gameCam = cam;
}

void Camera3D::SetCurrentCamera(ComponentCamera* cam)
{
	if (cam) currentCam = cam;
	else currentCam = editorCam;
}

void Camera3D::SetCurrentCamera(CameraType type)
{
	switch (type)
	{
		case CameraType::EDITOR: currentCam = editorCam; break;
		case CameraType::GAME: currentCam = gameCam; break;
	}
}

void Camera3D::CenterCameraToGO(AABB* boundBox)
{
	float dist = boundBox->Size().y / Tan(editorCam->GetVerticalFov() / 2);
	float3 dir = boundBox->CenterPoint() - position;

	if (Distance(boundBox->CenterPoint(), position) > dist + 0.5f)
	{
		position += dir.Normalized();
	}
	else if (Distance(boundBox->CenterPoint(), position) < dist - 0.5f)
	{
		position -= dir.Normalized();
	}
	else
	{
		focusing = false;
	}

	reference = boundBox->CenterPoint();
	editorCam->SetPosition(position);
	editorCam->Look(reference);
}

void Camera3D::OrbitGO(AABB* boundBox, float& dx, float& dy)
{
	reference = boundBox->CenterPoint();

	float3 dir = editorCam->GetPosition() - reference;

	math::Quat rotationX = math::Quat::RotateAxisAngle(editorCam->frustum.Up(), dx * DEGTORAD);
	math::Quat rotationY = math::Quat::RotateAxisAngle(editorCam->frustum.WorldRight(), dy * DEGTORAD);
	math::Quat finalRotation = rotationX * rotationY;

	dir = finalRotation.Transform(dir);

	position = dir + reference;
	editorCam->SetPosition(position);
	LookAt(reference);
}

void Camera3D::DrawInFrustumCulling(GameObject* go, ComponentCamera* camera)
{
	if (go->GetGlobalAABB()->IsFinite())
	{
		if (IntersectsAABB(go->GetGlobalAABB(), camera))
			go->insideFrustum = true;
		else
			go->insideFrustum = false;
	}
}

void Camera3D::PropagateDrawInFrustumCulling(GameObject* go, ComponentCamera* camera)
{
	std::queue<GameObject*> q;
	q.push(go);

	while (!q.empty())
	{
		auto& curr = q.front();
		q.pop();

		DrawInFrustumCulling(curr, camera);

		for (auto& child : curr->GetChilds())
			q.push(child);
	}
}

bool Camera3D::IntersectsAABB(const AABB* aabb, ComponentCamera* camera)
{
	float3 corners[8];
	aabb->GetCornerPoints(corners);

	Plane planes[6];
	camera->frustum.GetPlanes(planes);

	for (uint i = 0; i < 6; ++i)
	{
		uint pointInsideFrustum = 8;

		for (uint j = 0; j < 8; j++)
		{
			if (planes[i].IsOnPositiveSide(corners[j]))
			{
				pointInsideFrustum--;
			}
		}

		if (pointInsideFrustum == 0)
		{
			return false;
		}
	}

	return true;
}

void Camera3D::Save(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* camObj = json_object_get_object(root, name.c_str());

	json_object_set_value(camObj, "worldmatrix", json_value_init_object());
	JSON_Object* worldObj = json_object_get_object(camObj, "worldmatrix");

	float3x4 worldMat = editorCam->frustum.WorldMatrix();

	json_object_set_number(worldObj, "x0", worldMat.At(0, 0));
	json_object_set_number(worldObj, "y0", worldMat.At(1, 0));
	json_object_set_number(worldObj, "z0", worldMat.At(2, 0));

	json_object_set_number(worldObj, "x1", worldMat.At(0, 1));
	json_object_set_number(worldObj, "y1", worldMat.At(1, 1));
	json_object_set_number(worldObj, "z1", worldMat.At(2, 1));

	json_object_set_number(worldObj, "x2", worldMat.At(0, 2));
	json_object_set_number(worldObj, "y2", worldMat.At(1, 2));
	json_object_set_number(worldObj, "z2", worldMat.At(2, 2));

	json_object_set_number(worldObj, "x3", worldMat.At(0, 3));
	json_object_set_number(worldObj, "y3", worldMat.At(1, 3));
	json_object_set_number(worldObj, "z3", worldMat.At(2, 3));
}

void Camera3D::Load(JSON_Object* root)
{
	JSON_Object* camObj = json_object_get_object(root, name.c_str());
	JSON_Object* worldObj = json_object_get_object(camObj, "worldmatrix");

	float3 pos = { 0,0,0 };
	float3 rot = { 0,0,0 };
	float3x3 rotMat;

	rot.x = json_object_get_number(worldObj, "x0");
	rot.y = json_object_get_number(worldObj, "y0");
	rot.z = json_object_get_number(worldObj, "z0");

	rotMat.SetCol(0, rot);

	rot.x = json_object_get_number(worldObj, "x1");
	rot.y = json_object_get_number(worldObj, "y1");
	rot.z = json_object_get_number(worldObj, "z1");

	rotMat.SetCol(1, rot);

	rot.x = json_object_get_number(worldObj, "x2");
	rot.y = json_object_get_number(worldObj, "y2");
	rot.z = json_object_get_number(worldObj, "z2");

	rotMat.SetCol(2, rot);

	pos.x = json_object_get_number(worldObj, "x3");
	pos.y = json_object_get_number(worldObj, "y3");
	pos.z = json_object_get_number(worldObj, "z3");

	float3x4 worldMat{ rotMat, pos };
	editorCam->frustum.SetWorldMatrix(worldMat);
	position = pos;
}

void Camera3D::DrawPickingRay()
{
	if (debugDrawPicking)
	{
		glBegin(GL_LINES);
		glColor3f(0, 1, 0);

		glVertex3f(picking.a.x, picking.a.y, picking.a.z);
		glVertex3f(picking.b.x, picking.b.y, picking.b.z);

		glColor3f(1, 1, 1);
		glEnd();
	}
}

GameObject* Camera3D::MousePickGameObject()
{
	float4 winDimensions = app->editor->viewportPanel->GetDimensions();
	ImVec2 p = ImGui::GetIO().MousePos;
	int mouseX = app->input->GetMouseX();
	int mouseY = app->input->GetMouseY();

	printf("%f %f\n", p.x, p.y);
	printf("%i %i\n", mouseX, mouseY);


	ImVec2 mouseWinPos = ImVec2(p.x - winDimensions.x, p.y- winDimensions.y);
	float x = Lerp(-1.f, 1.f, mouseWinPos.x / winDimensions.z);
	float y = Lerp(1.f, -1.f, mouseWinPos.y / winDimensions.w);

	picking = editorCam->frustum.UnProjectLineSegment(x, y);

	float3 hitPoint;
	float distance;
	static bool clearVec = false;
	
	GameObject* ret = ThrowRay(picking, hitPoint, clearVec, distance, app->scene->GetRoot());

	return ret;
}

GameObject* Camera3D::ThrowRay(LineSegment line, float3& hitPoint, bool& clearVector, float& dist, GameObject* gameObject)
{
	std::vector<GameObject*> gos;
	std::vector<std::pair<GameObject*, float>> distances;

	std::queue<GameObject*> q;

	app->scene->rootQT->Intersect(gos, line);
	if (gos.empty())
		gos.insert(gos.begin(), gameObject->GetChilds().begin(), gameObject->GetChilds().end());

	if (!gos.empty())
	{
		for (auto& go : gos)
			q.push(go);

		while (!q.empty())
		{
			GameObject* curr = q.front();
			q.pop();

			LineSegment localLine = line;
			
			// Mesh
			if (ComponentMesh* meshComp = (ComponentMesh*)curr->GetComponent(ComponentType::MESH))
			{
				ComponentTransform* trans = (ComponentTransform*)curr->GetComponent(ComponentType::TRANSFORM);
				
				float4x4 m = trans->GetGlobalMatrix().Inverted();
				Ray ray = localLine.ToRay();
				ray.Transform(m);

				Triangle triangle;
				float distance = 0;
				for (int i = 0; i < meshComp->GetMesh()->indices.size(); i += 3)
				{
					triangle.a = meshComp->GetMesh()->vertices[meshComp->GetMesh()->indices[i]].position;
					triangle.b = meshComp->GetMesh()->vertices[meshComp->GetMesh()->indices[i + 1]].position;
					triangle.c = meshComp->GetMesh()->vertices[meshComp->GetMesh()->indices[i + 2]].position;

					if (ray.Intersects(triangle, &distance, &hitPoint))
					{
						std::pair<GameObject*, float> pair;
						pair.first = curr;
						pair.second = distance;
						distances.push_back(pair);
					}
				}
			}

			for (const auto& child : curr->GetChilds())
				q.push(child);
		}

		float min = 65553;
		GameObject* ret = 0;
		for (int i = 0; i < distances.size(); ++i)
		{
			if (distances[i].second < min)
			{
				ret = distances[i].first;
				min = distances[i].second;
			}
		}
		return ret;
	}

	return nullptr;
}