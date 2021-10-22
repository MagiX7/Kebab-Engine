#pragma once

#include "Component.h"

#include "Math/float3.h"
#include "Math/Quat.h"
#include "Math/float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject& compOwner);
	~ComponentTransform();

	void Enable();
	void Disable();

	void Translate(const float3& pos);
	void Rotate(const float3& rot);
	void Scalate(const float3& scal);

	void SetTranslation(const float3 newPos);
	void SetRotation(const float3 newRot);
	void SetScale(const float3 newScale);

	const float3& GetPosition() const { return position; }
	const float3& GetScale() const { return scale; }
	const float3& GetRotation() const { return rotation; }

	inline const float4x4& GetLocalMatrix() { return localTransformMat; }


private:
	void UpdateTransform();

	void DrawOnInspector();

private:

	float3 position;
	float3 scale;
	//Quat rotation;
	float3 rotation;

	float4x4 localTransformMat;
	float4x4 worldTransformMat;
};