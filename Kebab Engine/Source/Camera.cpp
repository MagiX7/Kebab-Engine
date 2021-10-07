#include "Camera.h"

Camera::Camera()
{
	fovVertical = 60.0f;
	fovHorizontal = 91.0f;

	planeFar = 200.f;
	planeNear = 0.1f;

	frustum.SetPerspective(fovHorizontal, fovVertical);

	frustum.SetFrame(vec(0, 0, 0), vec(0, 0, 1), vec(0, 1, 0));

	frustum.SetViewPlaneDistances(planeNear, planeFar);

	frustum.SetKind(math::FrustumProjectiveSpace::FrustumSpaceGL, math::FrustumHandedness::FrustumLeftHanded);

	frustum.ComputeViewProjMatrix();
}

Camera::~Camera()
{
}

void Camera::Look(const vec lookPoint)
{
	if (lookPoint.IsFinite())
	{
		vec direction = lookPoint - frustum.Pos();

		float3x3 matrix = float3x3::LookAt(frustum.Front(), direction.Normalized(), frustum.Up(), float3(0, 1, 0));

		frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
		frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
	}
}

// SETTERS =============================================
void Camera::SetVerticalFov(const float fovVert)
{
	frustum.SetVerticalFovAndAspectRatio(fovVert, 16 / 9);
}

void Camera::SetHorizontalFov(const float fovHoriz)
{
	frustum.SetHorizontalFovAndAspectRatio(fovHoriz, 16 / 9);
}

void Camera::SetNearPlane(const float nearPlane)
{
	frustum.SetViewPlaneDistances(nearPlane, planeFar);
	planeNear = nearPlane;
}

void Camera::SetFarPlane(const float farPlane)
{
	frustum.SetViewPlaneDistances(planeNear, farPlane);
	planeFar = farPlane;
}

void Camera::SetCameraPosition(const vec position)
{
	frustum.SetPos(position);
}

// GETTERS =============================================
Frustum* Camera::GetFrustum()
{
	return &frustum;
}

float4x4 Camera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

float3x4 Camera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float Camera::GetVerticalFov() const
{
	return fovVertical;
}

float Camera::GetHorizontalFov() const
{
	return fovHorizontal;
}

float Camera::GetNearPlane() const
{
	return planeNear;
}

float Camera::GetFarPlane() const
{
	return planeFar;
}

vec Camera::GetCameraPosition() const
{
	return frustum.Pos();
}