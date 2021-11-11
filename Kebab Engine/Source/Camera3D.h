#pragma once
#include "Module.h"

#include "Globals.h"

#include "Math/float3.h"

class GameObject;
class ComponentCamera;
class GameObject;

enum class CameraType
{
	EDITOR,
	GAME
};

class Camera3D : public Module
{
public:
	Camera3D(bool startEnabled = true);
	~Camera3D();

	bool Init(JSON_Object* root) override;
	bool Start() override;
	bool Update(float dt);
	bool CleanUp();

	void LookAt(const float3& point);
	void MoveTo(const float3& movement);
	void SetPosLook(const float3& pos, const float3& pointLook);
	void SetRatio(float ratio);

	float* GetViewMatrix();
	float* GetProjectionMatrix();
	ComponentCamera* GetCamera();
	void SetEditorCamera(ComponentCamera* cam);
	void SetGameCamera(ComponentCamera* cam);
	void SetCurrentCamera(CameraType type);

	void CenterCameraToGO(AABB* boundBox);
	void OrbitGO(AABB* boundBox, float& dx, float& dy);

	void DrawInFrustumCulling(GameObject* go, ComponentCamera* camera);
	void PropagateDrawInFrustumCulling(GameObject* go, ComponentCamera* camera);
	bool IntersectsAABB(const AABB* aabb, ComponentCamera* camera);

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

private:
	GameObject* MousePickGameObject();
	GameObject* ThrowRay(LineSegment& ray, float3& hitPoint, GameObject* go = nullptr);

public:

	float3 reference, position;

private:

	ComponentCamera* currentCam;
	ComponentCamera* editorCam;
	ComponentCamera* gameCam;

	bool focusing;
};