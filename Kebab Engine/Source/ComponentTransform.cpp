#include "ComponentTransform.h"

#include "GameObject.h"
#include "ComponentCamera.h"

#include "imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* compOwner)
{
	this->parent = compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;

	localTransformMat = float4x4::identity;
	worldTransformMat = float4x4::identity;

	guiPos = { 0,0,0 };
	guiRot = { 0,0,0 };
	guiScale = { 1,1,1 };
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
	float4x4 transformAABB = float4x4::FromTRS(position, rotation, scale);
	ImVec2 size = ImGui::GetWindowSize();

	float len = parent->GetName().size();
	len *= ImGui::GetFontSize();
	float center = (size.x / 2) - (len / 2);

	ImGui::SetNextItemWidth(center);
	
	ImGui::TextColored({0.1f, 0.9f, 0.9f, 1.0f}, parent->GetName().c_str());
	ImGui::Separator();


	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position");
		if (ImGui::DragFloat3("position", guiPos.ptr(), 0.05f))
		{
			SetTranslation(guiPos);
			PropagateTransform(parent, position, rotation, scale);
			transformAABB = float4x4::FromTRS(position, rotation, scale);
			parent->UpdateAABB(transformAABB);
			ComponentCamera* parentCam = (ComponentCamera*)parent->GetComponent(ComponentType::CAMERA);
			if (parentCam != nullptr)
				parentCam->SetCameraPosition(guiPos);
		}

		ImGui::Separator();

		ImGui::Text("Rotation");
		if (ImGui::DragFloat3("rotation", guiRot.ptr(), 0.05f))
		{
			Quat x = Quat::RotateX(math::DegToRad(guiRot.x));
			Quat y = Quat::RotateY(math::DegToRad(guiRot.y));
			Quat z = Quat::RotateZ(math::DegToRad(guiRot.z));

			SetRotation(x * y * z);
			PropagateTransform(parent, position, rotation, scale);
			transformAABB = float4x4::FromTRS(position, rotation, scale);
			parent->UpdateAABB(transformAABB);
		}

		ImGui::Separator();

		ImGui::Text("Scale");
		if(ImGui::DragFloat3("scale", guiScale.ptr(), 0.05f))
		{
			SetScale(guiScale);
			PropagateTransform(parent, position, rotation, scale);
			transformAABB = float4x4::FromTRS(position, rotation, scale);
			parent->UpdateAABB(transformAABB);
		}
	}
}

JSON_Value* ComponentTransform::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	Parser::DotSetObjectNumber(obj, "Type", 0);

	Parser::DotSetObjectNumber(obj, "Position.x", position.x);
	Parser::DotSetObjectNumber(obj, "Position.y", position.y);
	Parser::DotSetObjectNumber(obj, "Position.z", position.z);
	
	Parser::DotSetObjectNumber(obj, "Rotation.x", rotation.x);
	Parser::DotSetObjectNumber(obj, "Rotation.y", rotation.y);
	Parser::DotSetObjectNumber(obj, "Rotation.z", rotation.z);
	Parser::DotSetObjectNumber(obj, "Rotation.w", rotation.w);

	Parser::DotSetObjectNumber(obj, "Scale.x", scale.x);
	Parser::DotSetObjectNumber(obj, "Scale.y", scale.y);
	Parser::DotSetObjectNumber(obj, "Scale.z", scale.z);

	// TODO: Save view matrix

	return value;
}

void ComponentTransform::Load(JSON_Object* obj, GameObject* parent)
{
	position.x = json_object_dotget_number(obj, "Position.x");
	position.y = json_object_dotget_number(obj, "Position.y");
	position.z = json_object_dotget_number(obj, "Position.z");

	rotation.x = json_object_dotget_number(obj, "Rotation.x");
	rotation.y = json_object_dotget_number(obj, "Rotation.y");
	rotation.z = json_object_dotget_number(obj, "Rotation.z");
	rotation.w = json_object_dotget_number(obj, "Rotation.w");

	scale.x = json_object_dotget_number(obj, "Scale.x");
	scale.y = json_object_dotget_number(obj, "Scale.y");
	scale.z = json_object_dotget_number(obj, "Scale.z");

	localTransformMat = float4x4::FromTRS(position, rotation, scale);
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

		ComponentCamera* childCam = (ComponentCamera*)(*it)->GetComponent(ComponentType::CAMERA);

		if (childCam != nullptr)
			childCam->SetCameraPosition(childTrans->GetTranslation());
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
	localTransformMat = float4x4::FromTRS(position, rotation, newScale);
}