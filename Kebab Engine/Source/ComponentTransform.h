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
	void Rotate(const Quat& rot);
	void Scalate(const float3& scal);

	void SetTranslation(const float3& newPos);
	void SetRotation(const Quat& newRot);
	void SetScale(const float3& newScale);

	const float3& GetTranslation() const { return position; }
	const float3& GetScale() const { return scale; }
	const Quat& GetRotation() const { return rotation; }

	inline const float4x4& GetLocalMatrix() { return localTransformMat; }


private:
	void UpdateTransform(float4x4 newTransform);

	void PropagateTransform(GameObject* go, float3& newPos, Quat& quat, float3& scale);

	void DrawOnInspector();

private:

	float3 position;
	float3 scale;
	//Quat rotation;
	Quat rotation;

	float4x4 localTransformMat;
	float4x4 worldTransformMat;


	float3 guiPos;
	float3 guiRot;
};