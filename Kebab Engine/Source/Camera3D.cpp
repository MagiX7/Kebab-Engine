#include "Application.h"
#include "Camera3D.h"

#include "mmgr/mmgr.h"

Camera3D::Camera3D(bool startEnabled) : Module(startEnabled)
{
	name = "camera3D";

	CalculateViewMatrix();

	x = vec3(1.0f, 0.0f, 0.0f);
	y = vec3(0.0f, 1.0f, 0.0f);
	z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 0.0f, 5.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);
}

Camera3D::~Camera3D()
{}

bool Camera3D::Init(JSON_Object* root)
{

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
	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= z * speed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += z * speed;


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= x * speed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += x * speed;

	position += newPos;
	reference += newPos;

	// Mouse motion ----------------
	if(app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		position -= reference;

		if (dx != 0)
		{
			float deltaX = (float)dx * sensitivity;

			x = rotate(x, deltaX, vec3(0.0f, 1.0f, 0.0f));
			y = rotate(y, deltaX, vec3(0.0f, 1.0f, 0.0f));
			z = rotate(z, deltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * sensitivity;

			y = rotate(y, DeltaY, x);
			z = rotate(z, DeltaY, x);

			if (y.y < 0.0f)
			{
				z = vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				y = cross(z, x);
			}
		}

		position = reference + z * length(position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return true;
}

// -----------------------------------------------------------------
void Camera3D::Look(const vec3& pos, const vec3& ref, bool rotateAroundReference)
{
	this->position = pos;
	this->reference = ref;

	z = normalize(pos - ref);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	if(!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void Camera3D::LookAt( const vec3& spot)
{
	reference = spot;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void Camera3D::Move(const vec3& movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* Camera3D::GetViewMatrix()
{
	return &viewMatrix;
}

void Camera3D::Save(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* camObj = json_object_get_object(root, name.c_str());

	json_object_set_number(camObj, "pos x", position.x);
	json_object_set_number(camObj, "pos y", position.y);
	json_object_set_number(camObj, "pos z", position.z);


}

// -----------------------------------------------------------------
void Camera3D::CalculateViewMatrix()
{
	viewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}
