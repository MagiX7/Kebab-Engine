#pragma once

#include "MathGeoLib.h"
#include "Geometry/Frustum.h"

class Camera
{
public:
	Camera();
	~Camera();

	void SetCameraPosition(const vec& position);
	vec GetCameraPosition() const;

	void Look(const vec& lookPoint);

	Frustum* GetFrustum();

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;

	void SetVerticalFov(const float& fovVert);
	void SetHorizontalFov(const float& fovHoriz);
	void SetFarPlane(const float& farPlane);
	void SetNearPlane(const float& nearPlane);

	float GetVerticalFov() const;
	float GetHorizontalFov() const;
	float GetFarPlane() const;
	float GetNearPlane() const;

	Frustum frustum;

private:

	float fovVertical;
	float fovHorizontal;

	float planeFar;
	float planeNear;
};