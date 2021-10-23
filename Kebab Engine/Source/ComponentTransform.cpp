#include "ComponentTransform.h"

#include "imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;

	localTransformMat = float4x4::identity;
	worldTransformMat = float4x4::identity;
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Enable()
{
	this->active = true;
}

void ComponentTransform::Disable()
{
	this->active = false;
}

void ComponentTransform::DrawOnInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position");
		if (ImGui::DragFloat3("position", position.ptr(), 0.05f))
		{
			SetTranslation(position);
		}

		ImGui::Separator();

		ImGui::Text("Rotation");
		static float3 newRot = { 0,0,0 };
		if (ImGui::DragFloat3("rotation", newRot.ptr(), 0.05f))
		{
			Quat x = Quat::RotateX(math::DegToRad(newRot.x));
			Quat y = Quat::RotateY(math::DegToRad(newRot.y));
			Quat z = Quat::RotateZ(math::DegToRad(newRot.z));

			SetRotation(x * y * z);
		}

		ImGui::Separator();

		ImGui::Text("Scale");
		if(ImGui::DragFloat3("scale", scale.ptr(), 0.05f))
		{
			SetScale(scale);
		}
	}
}

void ComponentTransform::UpdateTransform()
{

}

void ComponentTransform::Translate(const float3& pos)
{
	localTransformMat = localTransformMat * float4x4::FromEulerXYZ(pos.x, pos.y, pos.z);

	position += pos;
}

void ComponentTransform::Rotate(const Quat& rot)
{
	localTransformMat = localTransformMat * float4x4::FromQuat(rot);

	rotation.x += rot.x;
	rotation.y += rot.y;
	rotation.z += rot.z;
	rotation.w += rot.w;
}

void ComponentTransform::Scalate(const float3& scal)
{
	localTransformMat = localTransformMat * float4x4::FromEulerXYZ(scal.x, scal.y, scal.z);

	scale += scal;
}

void ComponentTransform::SetTranslation(const float3& newPos)
{
	position = newPos;

	//float4x4 r = float4x4::FromQuat(rotation);
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::SetRotation(const Quat& newRot)
{
	//float len = newRot.Normalize();
	//rotation = newRot;

	//rotation = Quat::FromEulerXYX(newRot.x, newRot.y, newRot.z);

	//rotation = newRot;

	//float4x4 r = float4x4::FromQuat(rotation);
	localTransformMat = float4x4::FromTRS(position, newRot, scale);
}

void ComponentTransform::SetScale(const float3& newScale)
{
	scale = newScale;

	float4x4 r = float4x4::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	localTransformMat = float4x4::FromTRS(position, r, scale);
}