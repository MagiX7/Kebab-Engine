#pragma once
#include "Component.h"

#include "MathGeoLib.h"
#include "Geometry/Frustum.h"
#include "GameObject.h"

enum class CameraType
{
	NONE = -1,
	EDITOR,
	GAME
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* compOwner, CameraType camerType);
	~ComponentCamera();

	void SetPosition(const vec& position);
	vec GetPosition();

	void Look(const vec& lookPoint);

	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;

	void SetVerticalFov(const float& fovVert);
	void SetHorizontalFov(const float& fovHoriz);
	void SetFarPlane(const float& farPlane);
	void SetNearPlane(const float& nearPlane);

	void CalculateFov(float w, float h, int horizontalFov = 0);
	void CalculateFov();


	float GetVerticalFov() const;
	float GetHorizontalFov() const;
	float GetFarPlane() const;
	float GetNearPlane() const;
	inline bool CameraToCurrent() { return cameraActive; }
	inline void DisableFromCurent() { cameraActive = false; }

	void DrawOnInspector() override;

	void DrawFrustum();

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;


public:
	CameraType cameraType;
	Frustum frustum;

private:

	float fovVertical;
	float fovHorizontal;

	int currentWinWidth;
	int currentWinHeight;

	float planeFar;
	float initialPlaneFar;
	float planeNear;
	float initialPlaneNear;

	bool cameraActive;
};