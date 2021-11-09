#pragma once
#include "Component.h"

#include "MathGeoLib.h"
#include "Geometry/Frustum.h"
#include "GameObject.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject& compOwner);
	~ComponentCamera();

	void SetCameraPosition(const vec& position);
	vec GetCameraPosition() const;

	void Look(const vec& lookPoint);

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

	void DrawOnInspector() override;

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

	Frustum frustum;

	bool frustumCulling;

private:

	float fovVertical;
	float fovHorizontal;

	float planeFar;
	float planeNear;

	bool cameraActive;
};