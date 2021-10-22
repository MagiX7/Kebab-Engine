#include "ComponentTransform.h"

#include "imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;
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

void ComponentTransform::SetPosition(const float3& pos)
{
	position = pos;
}

void ComponentTransform::SetRotation(const Quat& rot)
{
	rotation = rot;
}

void ComponentTransform::SetScale(const float3& scal)
{
	scale = scal;
}