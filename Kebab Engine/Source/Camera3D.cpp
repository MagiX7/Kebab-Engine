#include "Application.h"
#include "Camera3D.h"

#include "Camera.h"

#include "mmgr/mmgr.h"

Camera3D::Camera3D(bool startEnabled) : Module(startEnabled)
{
	name = "camera3D";

	position = vec(0.0f, 0.0f, 0.0f);
	reference = vec(0.0f, 0.0f, 0.0f);

	cam = new Camera();

	cam->SetCameraPosition(position);
	cam->Look(reference);
}

Camera3D::~Camera3D()
{}

bool Camera3D::Init(JSON_Object* root)
{
	/*JSON_Object* camObj = json_object_get_object(root, name.c_str());
	JSON_Object* posObj = json_object_get_object(camObj, "position");
	position.x = json_object_get_number(posObj, "x");
	position.y = json_object_get_number(posObj, "y");
	position.z = json_object_get_number(posObj, "z");

	JSON_Object* refObj = json_object_get_object(camObj, "reference");
	reference.x = json_object_get_number(refObj, "x");
	reference.y = json_object_get_number(refObj, "y");
	reference.z = json_object_get_number(refObj, "z");

	JSON_Object* rotObj = json_object_get_object(camObj, "rotation");
	JSON_Object* xObj = json_object_get_object(rotObj, "x");
	x.x = json_object_get_number(xObj, "x");
	x.y = json_object_get_number(xObj, "y");
	x.z = json_object_get_number(xObj, "z");

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

	return true;
}

// -----------------------------------------------------------------
bool Camera3D::Update(float dt)
{	
	float3 newPos(0, 0, 0);
	float speed = 20.0f * dt;
  
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 40.0f * dt;

	// Movement of camera ====================================================================================
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cam->frustum.Front() * speed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cam->frustum.Front() * speed;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cam->frustum.WorldRight() * speed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cam->frustum.WorldRight() * speed;

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= cam->frustum.Up() * speed * 0.5f;
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += cam->frustum.Up() * speed * 0.5f;

	position += newPos;
	reference += newPos;

	cam->SetCameraPosition(position);

	// Rotation of camera ================================================================================
	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) 
	{
		float dx = -app->input->GetMouseXMotion();
		float dy = -app->input->GetMouseYMotion();

		math::Quat rotationX = math::Quat::RotateAxisAngle(float3::unitY, dx * DEGTORAD);
		math::Quat rotationY = math::Quat::RotateAxisAngle(cam->frustum.WorldRight(), dy * DEGTORAD);
		math::Quat finalRotation = rotationX * rotationY;

		cam->frustum.Transform(finalRotation);
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

void Camera3D::SetPosLook(const float3& pos, float3& point)
{
	cam->Look(point);
	reference = point;
	cam->SetCameraPosition(pos);
	position = pos;
}

float* Camera3D::GetViewMatrix()
{
	return cam->GetViewMatrix().ptr();
}

void Camera3D::Save(JSON_Object* root)
{
	/*json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* camObj = json_object_get_object(root, name.c_str());

	json_object_set_value(camObj, "position", json_value_init_object());
	JSON_Object* posObj = json_object_get_object(camObj, "position");
	json_object_set_number(posObj, "x", position.x);
	json_object_set_number(posObj, "y", position.y);
	json_object_set_number(posObj, "z", position.z);


	json_object_set_value(camObj, "rotation", json_value_init_object());
	JSON_Object* rotObj = json_object_get_object(camObj, "rotation");
	
	json_object_set_value(rotObj, "x", json_value_init_object());
	JSON_Object* xObj = json_object_get_object(rotObj, "x");
	json_object_set_number(xObj, "x", x.x);
	json_object_set_number(xObj, "y", x.y);
	json_object_set_number(xObj, "z", x.z);

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

	json_object_set_value(camObj, "reference", json_value_init_object());
	JSON_Object* refObj = json_object_get_object(camObj, "reference");
	json_object_set_number(refObj, "x", reference.x);
	json_object_set_number(refObj, "y", reference.y);
	json_object_set_number(refObj, "z", reference.z);


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
