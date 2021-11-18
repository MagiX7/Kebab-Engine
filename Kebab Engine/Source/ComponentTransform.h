#pragma once

#include "Component.h"

#include "Math/float3.h"
#include "Math/Quat.h"
#include "Math/float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* compOwner);
	~ComponentTransform();

	void Enable();
	void Disable();

	void Translate(const float3& pos);
	void Rotate(const Quat& rot);
	void Scalate(const float3& scal);

	void SetTranslation(const float3& newPos);
	void SetRotation(const Quat& newRot);
	void SetScale(const float3& newScale);

	float3& GetTranslation() { return position; }
	float3& GetScale() { return scale; }
	Quat& GetRotation() { return rotation; }

	// Transform matrix
	inline float4x4& GetLocalMatrix() { return localTransformMat; }
	inline float4x4& GetGlobalMatrix() { return globalTransformMat; }

	void SetLocalMatrix(const float4x4& transform);

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

	void PropagateTransform(GameObject* go, float3& newPos, Quat& quat, float3& scale);

private:
	void UpdateTransform(float4x4 newTransform);

	void RecomputeGlobalMat();


	void DrawOnInspector();

private:

	float3 position;
	float3 scale;
	Quat rotation;

	float4x4 localTransformMat;
	float4x4 globalTransformMat;


	float3 guiPos;
	float3 guiRot;
	float3 guiScale;
};