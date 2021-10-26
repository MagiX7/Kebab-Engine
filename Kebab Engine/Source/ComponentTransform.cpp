#include "ComponentTransform.h"

#include "GameObject.h"

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
	ImVec2 size = ImGui::GetWindowSize();

	float len = parent->GetName().size();
	len *= ImGui::GetFontSize();
	float center = (size.x / 2) - (len / 2);

	ImGui::SetNextItemWidth(center);
	
	ImGui::TextColored({0.1f, 0.9f, 0.9f, 1.0f}, parent->GetName().c_str());
	ImGui::Separator();


	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static float3 newPos;
		static float3 newScale;

		ImGui::Text("Position");
		if (ImGui::DragFloat3("position", newPos.ptr(), 0.05f))
		{
			SetTranslation(newPos);
			//SetTranslation(position);

			PropagateTransform(parent, position, rotation, scale);

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
			PropagateTransform(parent, position, rotation, scale);
		}

		ImGui::Separator();

		ImGui::Text("Scale");
		if(ImGui::DragFloat3("scale", scale.ptr(), 0.05f))
		{
			SetScale(scale);
			PropagateTransform(parent, position, rotation, scale);
		}
	}
}

void ComponentTransform::UpdateTransform(float4x4 newTransform)
{
}

void ComponentTransform::PropagateTransform(GameObject* go, float3& newPos, Quat& newQuat, float3& newScale)
{
	std::vector<GameObject*>::iterator it = go->GetChilds().begin();
	for (; it != parent->GetChilds().end(); ++it)
	{
		ComponentTransform* childTrans = (ComponentTransform*)(*it)->GetComponent(ComponentType::TRANSFORM);
		ComponentTransform* parentTrans = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);

		if ((*it)->GetChilds().size() > 0)
		{
			PropagateTransform((*it), newPos, newQuat, newScale);
		}
		else
		{
			childTrans->SetTranslation(/*parentTrans->GetTranslation() + */newPos);
			childTrans->SetRotation(/*parentTrans->GetRotation() * */newQuat);
			childTrans->SetScale(/*parentTrans->GetScale() + */newScale);
		}
	}
}

void ComponentTransform::Translate(const float3& pos)
{
	position += pos;
	localTransformMat = float4x4::FromTRS(pos, rotation, scale);
}

void ComponentTransform::Rotate(const Quat& rot)
{
	rotation = rotation * rot;
	localTransformMat = float4x4::FromTRS(position, rot, scale);
}

void ComponentTransform::Scalate(const float3& scal)
{
	scale += scal;
	localTransformMat = float4x4::FromTRS(position, rotation, scal);
}

void ComponentTransform::SetTranslation(const float3& newPos)
{
	position = newPos;
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::SetRotation(const Quat& newRot)
{
	rotation = newRot;
	localTransformMat = float4x4::FromTRS(position, newRot, scale);
}

void ComponentTransform::SetScale(const float3& newScale)
{
	scale = newScale;
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
}