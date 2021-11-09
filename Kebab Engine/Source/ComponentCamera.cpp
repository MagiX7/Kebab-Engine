#include "Application.h"
#include "ComponentCamera.h"

#include "GameObject.h"

#include "imgui/imgui.h"

#include "mmgr/mmgr.h"

ComponentCamera::ComponentCamera()
{
	this->parent = nullptr;
	this->active = true;
	this->type = ComponentType::CAMERA;
	
	frustumCulling = true;

	fovVertical = 70.0f;
	fovHorizontal = 80.0f;

	planeFar = 200.f;
	planeNear = 1.f;

	frustum.SetPerspective(fovHorizontal, fovVertical);

	frustum.SetFrame(vec(0, 0, 0), vec(0, 0, 1), vec(0, 1, 0));

	frustum.SetViewPlaneDistances(planeNear, planeFar);

	frustum.SetKind(math::FrustumProjectiveSpace::FrustumSpaceGL, math::FrustumHandedness::FrustumRightHanded);
}

ComponentCamera::ComponentCamera(GameObject& compOwner)
{
	this->SetParent(&compOwner);
	this->active = true;
	this->type = ComponentType::CAMERA;

	frustumCulling = true;

	fovVertical = 70.0f;
	fovHorizontal = 80.0f;

	planeFar = 200.f;
	planeNear = 0.1f;

	frustum.SetPerspective(fovHorizontal, fovVertical);

	frustum.SetFrame(vec(0, 0, 0), vec(0, 0, 1), vec(0, 1, 0));

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

float ComponentCamera::GetFarPlane() const
{
	return planeFar;
}

vec ComponentCamera::GetCameraPosition() const
{
	return frustum.Pos();
}


void ComponentCamera::DrawOnInspector()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("Activate Camera", &cameraActive);
	}
}