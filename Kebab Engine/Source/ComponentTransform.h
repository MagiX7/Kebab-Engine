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

	void SetTranslation(const float3& newPos);
	void SetRotation(const float3& newRot);
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

	void TransformParentMoved();

private:

	void RecomputeGlobalMat();

	void DrawOnInspector();

private:

	float3 position;
	float3 scale;
	Quat rotation;
	float3 eulerRotation;

	float4x4 localTransformMat;
	float4x4 globalTransformMat;
};