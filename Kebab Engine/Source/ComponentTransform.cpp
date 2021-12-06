#include "ComponentTransform.h"

#include "GameObject.h"
#include "ComponentCamera.h"

#include "imgui/imgui.h"

#include "mmgr/mmgr.h"

ComponentTransform::ComponentTransform(GameObject* compOwner)
{
	this->parent = compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;

	localTransformMat = float4x4::identity;
	globalTransformMat = float4x4::identity;

	position = float3::zero;
	rotation = Quat::identity;
	scale = float3::one;

	RecomputeGlobalMat();
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


	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ComponentCamera* parentCam = (ComponentCamera*)parent->GetComponent(ComponentType::CAMERA);

		ImGui::Text("Position");
		float3 newPosition = position;
		if (ImGui::DragFloat3("position", newPosition.ptr(), 0.05f))
		{
			SetTranslation(newPosition);
		}

		ImGui::Separator();

		ImGui::Text("Rotation");
		float3 newEulerRotation;
		newEulerRotation.x = RADTODEG * eulerRotation.x;
		newEulerRotation.y = RADTODEG * eulerRotation.y;
		newEulerRotation.z = RADTODEG * eulerRotation.z;
		if (ImGui::DragFloat3("rotation", newEulerRotation.ptr(), 0.05f))
		{
			newEulerRotation.x = DEGTORAD * newEulerRotation.x;
			newEulerRotation.y = DEGTORAD * newEulerRotation.y;
			newEulerRotation.z = DEGTORAD * newEulerRotation.z;

			SetRotation(newEulerRotation);
		}

		ImGui::Separator();

		ImGui::Text("Scale");
		float3 newScale = scale;
		if(ImGui::DragFloat3("scale", newScale.ptr(), 0.05f))
		{
			SetScale(newScale);		
		}

		if (parentCam != nullptr)
		{
			float3x4 mat;
			mat.SetRotatePart(rotation);
			mat.SetTranslatePart(position);

			parentCam->frustum.SetWorldMatrix(mat);
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
	RecomputeGlobalMat();
}

void ComponentTransform::RecomputeGlobalMat()
{
	if (parent->GetParent() && parent->GetParent()->GetParent() != nullptr)
	{
		ComponentTransform* tr = (ComponentTransform*)parent->GetParent()->GetComponent(ComponentType::TRANSFORM);
		globalTransformMat = tr->globalTransformMat * localTransformMat;
	}
	else
	{
		globalTransformMat = localTransformMat;
	}
}


void ComponentTransform::SetLocalMatrix(const float4x4& transform)
{
	localTransformMat = transform;

	float3 newPos, newScale;
	Quat newRot;
	localTransformMat.Decompose(newPos, newRot, newScale);

	SetTranslation(newPos);
	SetRotation(newRot);
	SetScale(newScale);

	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::SetGlobalMatrix(const float4x4& transform)
{
	globalTransformMat = transform;

	if (parent->GetParent() && parent->GetParent()->GetParent() != nullptr)
	{
		ComponentTransform* tr = (ComponentTransform*)parent->GetParent()->GetComponent(ComponentType::TRANSFORM);
		localTransformMat = tr->globalTransformMat.Inverted() * globalTransformMat;
		SetLocalMatrix(localTransformMat);
	}

	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::TransformParentMoved()
{
	float3 newPos, newScale;
	Quat newRot;
	localTransformMat.Decompose(newPos, newRot, newScale);

	SetTranslation(newPos);
	SetRotation(newRot);
	SetScale(newScale);

	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::SetTranslation(const float3& newPos)
{
	position = newPos;
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::SetRotation(const float3& newRot)
{
	rotation = Quat::FromEulerXYZ(newRot.x, newRot.y, newRot.z);
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
	eulerRotation = newRot;
	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::SetRotation(const Quat& newRot)
{
	rotation = newRot;
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
	eulerRotation = rotation.ToEulerXYZ();
	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}

void ComponentTransform::SetScale(const float3& newScale)
{
	scale = newScale;
	localTransformMat = float4x4::FromTRS(position, rotation, scale);
	RecomputeGlobalMat();
	parent->PropagateTransform();
	parent->UpdateAABB(globalTransformMat);
}