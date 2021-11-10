#include "Application.h"
#include "Window.h"
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

#include "mmgr/mmgr.h"

Camera3D::Camera3D(bool startEnabled) : Module(startEnabled)
{
	name = "camera3D";

	position = vec(0.0f, 0.0f, 0.0f);
	reference = vec(0.0f, 0.0f, 0.0f);

	cam = new ComponentCamera(nullptr);

	focusing = false;

	cam->SetCameraPosition(position);
	cam->Look(reference);
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

	delete(cam);

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

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		// Movement of camera ====================================================================================
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cam->frustum.Front() * speed;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cam->frustum.Front() * speed;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cam->frustum.WorldRight() * speed;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cam->frustum.WorldRight() * speed;

		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= float3::unitY * speed * 0.5f;
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += float3::unitY * speed * 0.5f;

		position += newPos;

		cam->SetCameraPosition(position);

		// Rotation of camera ================================================================================
		math::Quat rotationX = math::Quat::RotateAxisAngle(float3::unitY, dx * DEGTORAD);
		math::Quat rotationY = math::Quat::RotateAxisAngle(cam->frustum.WorldRight(), dy * DEGTORAD);
		math::Quat finalRotation = rotationX * rotationY;

		cam->frustum.Transform(finalRotation);
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
			zoom -= cam->frustum.Front() * speed;
			position += zoom;
			cam->SetCameraPosition(position);
		}
		if (app->input->GetMouseZ() > 0)
		{
			zoom += cam->frustum.Front() * speed;
			position += zoom;
			cam->SetCameraPosition(position);
		}
	}
	
	// Mouse Picking
	if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && app->editor->viewportPanel->IsHovered())
	{
		GameObject* picked = MousePickGameObject();
		app->editor->hierarchyPanel->SetCurrent(picked);
		//app->editor->hierarchyPanel->currentGO = picked;
	}


	// Focus
	if (app->editor->hierarchyPanel->currentGO != nullptr)
	{
		GameObject* selectedGO = app->editor->hierarchyPanel->currentGO;
		ComponentTransform* compTransGO = (ComponentTransform*)selectedGO->GetComponent(ComponentType::TRANSFORM);

		AABB* boundBox = selectedGO->GetGlobalAABB();

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) focusing = true;

		if (focusing == true)
		{
			CenterCameraToGO(boundBox);
		}

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			OrbitGO(boundBox, dx, dy);
		}
	}

	// Frustum Culling
	std::vector<GameObject*>::iterator it;
	for (it = app->scene->GetGameObjects().begin(); it != app->scene->GetGameObjects().end(); ++it)
	{
		DrawInFrustumCulling((*it), app->renderer3D->currentCam);

		if ((*it)->GetChilds().size() != 0)
			PropagateDrawInFrustumCulling((*it), app->renderer3D->currentCam);
	}

	return true;
}

void Camera3D::LookAt( const float3& point)
{
	cam->Look(point);
	reference = point;
}

void Camera3D::MoveTo(const float3& pos)
{
	cam->SetCameraPosition(pos);
	position = pos;
}

void Camera3D::SetPosLook(const float3& pos, const float3& pointLook)
{
	cam->Look(pointLook);
	reference = pointLook;
	cam->SetCameraPosition(pos);
	position = pos;
}

void Camera3D::SetRatio(float ratio)
{
	cam->frustum.SetHorizontalFovAndAspectRatio(Atan(ratio * Tan(cam->frustum.VerticalFov() / 2)) * 2, ratio);
}

float* Camera3D::GetViewMatrix()
{
	return cam->GetViewMatrix().ptr();
}

float* Camera3D::GetProjectionMatrix()
{
	return cam->GetProjectionMatrix().ptr();
}

ComponentCamera* Camera3D::GetCamera()
{
	return cam;
}

void Camera3D::CenterCameraToGO(AABB* boundBox)
{
	float dist = boundBox->Size().y / Tan(cam->GetVerticalFov() / 2);;
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
	cam->SetCameraPosition(position);
	cam->Look(reference);
}

void Camera3D::OrbitGO(AABB* boundBox, float& dx, float& dy)
{
	reference = boundBox->CenterPoint();

	float3 dir = cam->GetCameraPosition() - reference;

	math::Quat rotationX = math::Quat::RotateAxisAngle(cam->frustum.Up(), dx * DEGTORAD);
	math::Quat rotationY = math::Quat::RotateAxisAngle(cam->frustum.WorldRight(), dy * DEGTORAD);
	math::Quat finalRotation = rotationX * rotationY;

	dir = finalRotation.Transform(dir);

	position = dir + reference;
	cam->SetCameraPosition(position);
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
	std::vector<GameObject*>::iterator it;

	for (it = go->GetChilds().begin(); it != go->GetChilds().end(); it++)
	{
		DrawInFrustumCulling((*it), camera);

		if ((*it)->GetChilds().size() != 0)
			PropagateDrawInFrustumCulling((*it), camera);
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

	float3x4 worldMat = cam->frustum.WorldMatrix();

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
	cam->frustum.SetWorldMatrix(worldMat);
	position = pos;
}

ComponentMesh* Camera3D::GetComponentMeshFromChilds(GameObject* parent)
{
	for (auto& child : parent->GetChilds())
	{
		ComponentMesh* m = (ComponentMesh*)child->GetComponent(ComponentType::MESH);
		if (m)
			return m;

		if (child->GetChilds().size() > 0) GetComponentMeshFromChilds(child);
	}
	return nullptr;
}

GameObject* Camera3D::MousePickGameObject()
{
	float4 winDimensions = app->editor->viewportPanel->GetDimensions();
	/*int mouseX = app->input->GetMouseX();
	int mouseY = app->input->GetMouseY();*/

	ImVec2 p = ImGui::GetIO().MousePos;

	/*printf("ImGui Mouse  %f %f\n", p.x, p.y);
	printf("SDL Mouse  %i %i\n", mouseX, mouseY);*/


	/*float x = app->input->GetMouseX();
	float y = app->input->GetMouseY();*/
	
	/*float normalizedX = -(1.0f - (float(x) * 2.0f) / size.x);
	float normalizedY = 1.0f - (float(y) * 2.0f) / size.y;*/

	//ImVec2 mouseInWindowPos = ImVec2(mousePos.x - winDimensions.x - cursorX, mousePos.y - winDimensions.y - cursorY);
	ImVec2 mouseWinPos = ImVec2(p.x - winDimensions.x, p.y - winDimensions.y);
	float x = Lerp(-1.f, 1.f, mouseWinPos.x / winDimensions.z);
	float y = Lerp(1.f, -1.f, mouseWinPos.y / winDimensions.w);

	LineSegment picking = cam->frustum.UnProjectLineSegment(x, y);
	
	float distance;
	GameObject* hitted = ThrowRay(picking, distance);
	if (hitted)
		return hitted;

	return nullptr;
}

GameObject* Camera3D::ThrowRay(LineSegment& ray, float& distance)
{
	for (auto& go : app->scene->GetGameObjects())
	{
		/*ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);
		if (!mesh)
			mesh = GetComponentMeshFromChilds(go);
		if (mesh)
		{
			GameObject* meshParent = mesh->GetParent();
			while (meshParent->GetParent() != app->scene->GetRoot())
				meshParent = meshParent->GetParent();


			ComponentTransform* trans = (ComponentTransform*)meshParent->GetComponent(ComponentType::TRANSFORM);
			Triangle triangle;
			for (int i = 0; i < mesh->GetMesh()->indices.size(); i += 3)
			{
				triangle.a = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i]].position;
				triangle.b = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i + 1]].position;
				triangle.c = mesh->GetMesh()->vertices[mesh->GetMesh()->indices[i + 2]].position;

				Ray localRay = ray.ToRay();
				localRay.Transform(trans->GetGlobalMatrix().Inverted());
				if (localRay.Intersects(triangle))
				{
					return go;
				}
			}
		}*/

		
		ComponentTransform* trans = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);

		LineSegment localRay = ray;
		localRay.Transform(trans->GetGlobalMatrix().Inverted());
		if (go->GetGlobalAABB()->Intersects(localRay))
		{
			return go;
		}
	}

	return nullptr;
}