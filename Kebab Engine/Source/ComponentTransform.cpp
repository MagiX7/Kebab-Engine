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
		ImGui::SetNextItemWidth(75.f);
		bool change = ImGui::DragFloat("X", &position.x, 0.2f, -50.f, 50.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change |= ImGui::DragFloat("Y", &position.y, 0.2f, -50.f, 50.f);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.f);
		change |= ImGui::DragFloat("Z", &position.z, 0.2f, -50.f, 50.f);

		ImGui::Separator();

		ImGui::Text("Rotation");
		ImGui::TextWrapped("x: %.2f	y: %.2f	z: %.2f", rotation.x, rotation.y, rotation.z);

		ImGui::Separator();

		ImGui::Text("Scale");
		ImGui::TextWrapped("x: %.2f	y: %.2f	z: %.2f", scale.x, scale.y, scale.z);
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

void ComponentTransform::Rotate(const float3& rot)
{
	localTransformMat = localTransformMat * float4x4::FromEulerXYZ(rot.x, rot.y, rot.z);

	rotation += rot;	
}

void ComponentTransform::Scalate(const float3& scal)
{
	localTransformMat = localTransformMat * float4x4::FromEulerXYZ(scal.x, scal.y, scal.z);

	scale += scal;
}

void ComponentTransform::SetTranslation(const float3 newPos)
{
	position = newPos;

	float4x4 r = float4x4::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	localTransformMat = float4x4::FromTRS(position, r, scale);
}

void ComponentTransform::SetRotation(const float3 newRot)
{
	rotation = newRot;

	float4x4 r = float4x4::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	localTransformMat = float4x4::FromTRS(position, r, scale);
}

void ComponentTransform::SetScale(const float3 newScale)
{
	scale = newScale;

	float4x4 r = float4x4::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	localTransformMat = float4x4::FromTRS(position, r, scale);
}