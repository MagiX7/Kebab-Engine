#include "Application.h"
#include "ComponentCamera.h"

#include "GameObject.h"

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "mmgr/mmgr.h"

ComponentCamera::ComponentCamera(GameObject* compOwner, CameraType camerType)
{
	this->SetParent(compOwner);
	this->active = true;
	this->type = ComponentType::CAMERA;
	this->cameraType = cameraType;

	cameraActive = false;

	frustumCulling = true;

	fovVertical = 70.0f;
	fovHorizontal = 80.0f;

	planeFar = 20.f;
	planeNear = 0.1f;

	frustum.SetPerspective(fovHorizontal, fovVertical);

	frustum.SetFrame(vec(0, 0, 0), vec(1, 0, 0), vec(0, 1, 0));

	frustum.SetViewPlaneDistances(planeNear, planeFar);

	frustum.SetKind(math::FrustumProjectiveSpace::FrustumSpaceGL, math::FrustumHandedness::FrustumRightHanded);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Look(const vec& lookPoint)
{
	if (lookPoint.IsFinite())
	{
		vec direction = lookPoint - frustum.Pos();

		float3x3 matrix = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3(0, 1, 0));

		frustum.SetFrame(frustum.Pos(), matrix.MulDir(frustum.Front()).Normalized(), matrix.MulDir(frustum.Up()).Normalized());
	}
}

// SETTERS =============================================
void ComponentCamera::SetVerticalFov(const float& fovVert)
{
	frustum.SetVerticalFovAndAspectRatio(fovVert, 16 / 9);
}

void ComponentCamera::SetHorizontalFov(const float& fovHoriz)
{
	frustum.SetHorizontalFovAndAspectRatio(fovHoriz, 16 / 9);
}

void ComponentCamera::SetNearPlane(const float& nearPlane)
{
	frustum.SetViewPlaneDistances(nearPlane, planeFar);
	planeNear = nearPlane;
}

void ComponentCamera::SetFarPlane(const float& farPlane)
{
	frustum.SetViewPlaneDistances(planeNear, farPlane);
	planeFar = farPlane;
}

void ComponentCamera::SetCameraPosition(const vec& position)
{
	frustum.SetPos(position);
}

// GETTERS =============================================
float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	float4x4 mat = frustum.ViewMatrix();
	return mat.Transposed();
}

float ComponentCamera::GetVerticalFov() const
{
	return fovVertical;
}

float ComponentCamera::GetHorizontalFov() const
{
	return fovHorizontal;
}

float ComponentCamera::GetNearPlane() const
{
	return planeNear;
}

JSON_Value* ComponentCamera::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	json_object_set_number(obj, "Type", 4);
	
	json_object_set_value(obj, "worldmatrix", json_value_init_object());
	JSON_Object* worldObj = json_object_get_object(obj, "worldmatrix");

	float3x4 worldMat = frustum.WorldMatrix();

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

	return value;
}

void ComponentCamera::Load(JSON_Object* obj, GameObject* parent)
{
	/*float3 f = frustum.Front().Normalized();
	float3 u = frustum.Up().Normalized();
	float3::Orthonormalize(f, u);
	frustum.SetFront(f);
	frustum.SetUp(u);*/

	//JSON_Object* camObj = json_object_get_object(root, name.c_str());
	JSON_Object* worldObj = json_object_get_object(obj, "worldmatrix");

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
	frustum.SetWorldMatrix(worldMat);

	frustum.SetPos(pos);

	//float3 u = frustum.Up();
	//float3 f = frustum.Front();
	//float3::Orthonormalize(u, f);
	//frustum.SetFront(f);
	//frustum.SetUp(u);
}

float ComponentCamera::GetFarPlane() const
{
	return planeFar;
}

vec ComponentCamera::GetCameraPosition()
{
	return frustum.Pos();
}


void ComponentCamera::DrawOnInspector()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		//ImGui::Checkbox("Set as Current Camera", &cameraActive);
	}
}

void ComponentCamera::DrawFrustum()
{
	GLdouble corners[24] = { 
		frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z,
		frustum.CornerPoint(1).x ,frustum.CornerPoint(1).y ,frustum.CornerPoint(1).z,
		frustum.CornerPoint(2).x ,frustum.CornerPoint(2).y ,frustum.CornerPoint(2).z,
		frustum.CornerPoint(3).x ,frustum.CornerPoint(3).y ,frustum.CornerPoint(3).z,
		frustum.CornerPoint(4).x ,frustum.CornerPoint(4).y ,frustum.CornerPoint(4).z,
		frustum.CornerPoint(5).x ,frustum.CornerPoint(5).y ,frustum.CornerPoint(5).z,
		frustum.CornerPoint(6).x ,frustum.CornerPoint(6).y ,frustum.CornerPoint(6).z,
		frustum.CornerPoint(7).x ,frustum.CornerPoint(7).y ,frustum.CornerPoint(7).z };


	glBegin(GL_LINES);
	glVertex3d(corners[0], corners[1], corners[2]);
	glVertex3d(corners[3], corners[4], corners[5]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(corners[0], corners[1], corners[2]);
	glVertex3d(corners[6], corners[7], corners[8]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(corners[0], corners[1], corners[2]);
	glVertex3d(corners[12], corners[13], corners[14]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[3], corners[4], corners[5]);
	glVertex3d(corners[9], corners[10], corners[11]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(corners[3], corners[4], corners[5]);
	glVertex3d(corners[15], corners[16], corners[17]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[6], corners[7], corners[8]);
	glVertex3d(corners[9], corners[10], corners[11]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(corners[6], corners[7], corners[8]);
	glVertex3d(corners[18], corners[19], corners[20]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[9], corners[10], corners[11]);
	glVertex3d(corners[21], corners[22], corners[23]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[12], corners[13], corners[14]);
	glVertex3d(corners[15], corners[16], corners[17]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(corners[12], corners[13], corners[14]);
	glVertex3d(corners[18], corners[19], corners[20]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[15], corners[16], corners[17]);
	glVertex3d(corners[21], corners[22], corners[23]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(corners[18], corners[19], corners[20]);
	glVertex3d(corners[21], corners[22], corners[23]);
	glEnd();
}