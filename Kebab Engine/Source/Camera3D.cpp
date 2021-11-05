#include "Application.h"
#include "Camera3D.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "mmgr/mmgr.h"

Camera3D::Camera3D(bool startEnabled) : Module(startEnabled)
{
	name = "camera3D";

	position = vec(0.0f, 0.0f, 0.0f);
	reference = vec(0.0f, 0.0f, 0.0f);

	cam = new ComponentCamera();

	focusing = false;

	cam->SetCameraPosition(position);
	cam->Look(reference);
}

Camera3D::~Camera3D()
{}

bool Camera3D::Init(JSON_Object* root)
{
	Load(root);

	/*float3 front;
	JSON_Object* frontObj = json_object_get_object(camObj, "front");
	front.x = json_object_get_number(frontObj, "x");
	front.y = json_object_get_number(frontObj, "y");
	front.z = json_object_get_number(frontObj, "z");
	cam->frustum.SetFront(front);*/

	/*JSON_Object* rotObj = json_object_get_object(camObj, "rotation");
	rotation.x = json_object_get_number(rotObj, "x");
	rotation.y = json_object_get_number(rotObj, "y");
	rotation.z = json_object_get_number(rotObj, "z");
	rotation.w = json_object_get_number(rotObj, "w");*/
	//cam->frustum.Transform(rotation);

	/*
	JSON_Object* yObj = json_object_get_object(rotObj, "y");
	y.x = json_object_get_number(yObj, "x");
	y.y = json_object_get_number(yObj, "y");
	y.z = json_object_get_number(yObj, "z");

	JSON_Object* zObj = json_object_get_object(rotObj, "z");
	z.x = json_object_get_number(zObj, "x");
	z.y = json_object_get_number(zObj, "y");
	z.z = json_object_get_number(zObj, "z");


	JSON_Object* viewObj = json_object_get_object(camObj, "view matrix");
	JSON_Object* vec1Obj = json_object_get_object(viewObj, "vector 1");
	viewMatrix.M[0] = json_object_get_number(vec1Obj, "x");
	viewMatrix.M[4] = json_object_get_number(vec1Obj, "y");
	viewMatrix.M[8] = json_object_get_number(vec1Obj, "z");
	viewMatrix.M[12] = json_object_get_number(vec1Obj, "w");

	JSON_Object* vec2Obj = json_object_get_object(viewObj, "vector 2");
	viewMatrix.M[1] = json_object_get_number(vec2Obj, "x");
	viewMatrix.M[5] = json_object_get_number(vec2Obj, "y");
	viewMatrix.M[9] = json_object_get_number(vec2Obj, "z");
	viewMatrix.M[13] = json_object_get_number(vec2Obj, "w");

	JSON_Object* vec3Obj = json_object_get_object(viewObj, "vector 3");
	viewMatrix.M[2] = json_object_get_number(vec3Obj, "x");
	viewMatrix.M[6] = json_object_get_number(vec3Obj, "y");
	viewMatrix.M[10] = json_object_get_number(vec3Obj, "z");
	viewMatrix.M[14] = json_object_get_number(vec3Obj, "w");

	JSON_Object* vec4Obj = json_object_get_object(viewObj, "vector 4");
	viewMatrix.M[3] = json_object_get_number(vec4Obj, "x");
	viewMatrix.M[7] = json_object_get_number(vec4Obj, "y");
	viewMatrix.M[11] = json_object_get_number(vec4Obj, "z");
	viewMatrix.M[15] = json_object_get_number(vec4Obj, "w");

	viewMatrixInverse = inverse(viewMatrix);*/

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
	float4 viewDim = app->editor->viewportPanel->GetViewportDimensions();

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
	
	if (app->editor->hierarchyPanel->currentGO != nullptr)
	{
		GameObject* selectedGO = app->editor->hierarchyPanel->currentGO;
		ComponentTransform* compTransGO = (ComponentTransform*)selectedGO->GetComponent(ComponentType::TRANSFORM);

		selectedGO->SetGlobalAABB(selectedGO);
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



	/*json_object_set_value(camObj, "position", json_value_init_object());
	JSON_Object* posObj = json_object_get_object(camObj, "position");
	json_object_set_number(posObj, "x", position.x);
	json_object_set_number(posObj, "y", position.y);
	json_object_set_number(posObj, "z", position.z);*/


	/*float3 front = cam->frustum.Front();
	json_object_set_value(camObj, "front", json_value_init_object());
	JSON_Object* frontObj = json_object_get_object(camObj, "front");
	json_object_set_number(frontObj, "x", front.x);
	json_object_set_number(frontObj, "y", front.y);
	json_object_set_number(frontObj, "z", front.z);*/

	/*
	json_object_set_value(rotObj, "y", json_value_init_object());
	JSON_Object* yObj = json_object_get_object(rotObj, "y");
	json_object_set_number(yObj, "x", y.x);
	json_object_set_number(yObj, "y", y.y);
	json_object_set_number(yObj, "z", y.z);

	json_object_set_value(rotObj, "z", json_value_init_object());
	JSON_Object* zObj = json_object_get_object(rotObj, "z");
	json_object_set_number(zObj, "x", z.x);
	json_object_set_number(zObj, "y", z.y);
	json_object_set_number(zObj, "z", z.z);

	json_object_set_value(camObj, "view matrix", json_value_init_object());
	JSON_Object* viewObj = json_object_get_object(camObj, "view matrix");
	json_object_set_value(viewObj, "vector 1", json_value_init_object());
	JSON_Object* vec1Obj = json_object_get_object(viewObj, "vector 1");

	json_object_set_number(vec1Obj, "x", viewMatrix.M[0]);
	json_object_set_number(vec1Obj, "y", viewMatrix.M[4]);
	json_object_set_number(vec1Obj, "z", viewMatrix.M[8]);
	json_object_set_number(vec1Obj, "w", viewMatrix.M[12]);

	json_object_set_value(viewObj, "vector 2", json_value_init_object());
	JSON_Object* vec2Obj = json_object_get_object(viewObj, "vector 2");

	json_object_set_number(vec2Obj, "x", viewMatrix.M[1]);
	json_object_set_number(vec2Obj, "y", viewMatrix.M[5]);
	json_object_set_number(vec2Obj, "z", viewMatrix.M[9]);
	json_object_set_number(vec2Obj, "w", viewMatrix.M[13]);

	json_object_set_value(viewObj, "vector 3", json_value_init_object());
	JSON_Object* vec3Obj = json_object_get_object(viewObj, "vector 3");

	json_object_set_number(vec3Obj, "x", viewMatrix.M[2]);
	json_object_set_number(vec3Obj, "y", viewMatrix.M[6]);
	json_object_set_number(vec3Obj, "z", viewMatrix.M[10]);
	json_object_set_number(vec3Obj, "w", viewMatrix.M[14]);

	json_object_set_value(viewObj, "vector 4", json_value_init_object());
	JSON_Object* vec4Obj = json_object_get_object(viewObj, "vector 4");

	json_object_set_number(vec4Obj, "x", viewMatrix.M[3]);
	json_object_set_number(vec4Obj, "y", viewMatrix.M[7]);
	json_object_set_number(vec4Obj, "z", viewMatrix.M[11]);
	json_object_set_number(vec4Obj, "w", viewMatrix.M[15]);*/

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


	//JSON_Object* posObj = json_object_get_object(camObj, "position");
	//position.x = json_object_get_number(posObj, "x");
	//position.y = json_object_get_number(posObj, "y");
	//position.z = json_object_get_number(posObj, "z");
	//cam->SetCameraPosition(position);
	//cam->Look({ 0, 0, 0 });

	/*float3 front;
	JSON_Object* frontObj = json_object_get_object(camObj, "front");
	front.x = json_object_get_number(frontObj, "x");
	front.y = json_object_get_number(frontObj, "y");
	front.z = json_object_get_number(frontObj, "z");
	cam->frustum.SetFront(front);*/

	//float3 up;
	//JSON_Object* upObj = json_object_get_object(camObj, "front");
	//up.x = json_object_get_number(upObj, "x");
	//up.y = json_object_get_number(upObj, "y");
	//up.z = json_object_get_number(upObj, "z");
	//cam->frustum.SetFrame(position, front, up);
}
