#pragma once
#include "Module.h"

#include "Globals.h"

#include "Math/float3.h"

class GameObject;
class ComponentCamera;
class ComponentMesh;

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

	void CenterCameraToGO(AABB* boundBox);
	void OrbitGO(AABB* boundBox, float& dx, float& dy);

	void DrawInFrustumCulling(GameObject* go);
	void PropagateDrawInFrustumCulling(GameObject* go);
	bool IntersectsAABB(const AABB* aabb);

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	ComponentMesh* GetComponentMeshFromChilds(GameObject* parent);

private:
	GameObject* MousePickGameObject();
	GameObject* ThrowRay(LineSegment& ray, float& distance);

public:
	float3 reference, position;

private:

	ComponentCamera* cam;

	bool focusing;
};