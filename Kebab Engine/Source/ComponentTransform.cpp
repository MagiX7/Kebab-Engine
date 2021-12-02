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
	globalTransformMat = float4x4::identity;

	guiPos = { 0,0,0 };
	guiRot = { 0,0,0 };
	guiScale = { 1,1,1 };

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
		if (ImGui::DragFloat3("position", guiPos.ptr(), 0.05f))
		{
			float3 translation = guiPos - position;

			SetTranslation(guiPos);
			PropagateTransform(parent, translation, rotation, scale);

			parent->HasMoved();
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

			parent->HasMoved();
		}

		ImGui::Separator();

		ImGui::Text("Scale");
		if(ImGui::DragFloat3("scale", guiScale.ptr(), 0.05f))
		{
			SetScale(guiScale);
			PropagateTransform(parent, position, rotation, scale);

			parent->HasMoved();
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

void ComponentTransform::SetLocalMatrix(const float4x4& transform)
{
	float3 prevPos, prevScale;
	Quat prevRot;

	localTransformMat.Decompose(prevPos, prevRot, prevScale);

	localTransformMat = transform;

	transform.Decompose(position, rotation, scale);

	float3 translation = position - prevPos;

	guiPos = position;
	//guiRot = rotation.ToEulerXYX();
	float3 r = { rotation.x, rotation.y, rotation.z };
	//guiRot = r.FromScalar(math::RadToDeg(rotation.Angle()));
	//guiRot = r.FromScalar(math::RadToDeg(rotation.Angle()));

	//guiRot = math::RadToDeg(guiRot);
	guiRot = { rotation.x, rotation.y, rotation.z };
	guiScale = scale;

	PropagateTransform(parent, translation, rotation, scale);
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

void ComponentTransform::UpdateTransform(float4x4 newTransform)
{
}

void ComponentTransform::RecomputeGlobalMat()
{
	if (parent->GetParent())
	{
		ComponentTransform* tr = (ComponentTransform*)parent->GetParent()->GetComponent(ComponentType::TRANSFORM);
		globalTransformMat = tr->globalTransformMat * localTransformMat;
	}
	else
	{
		globalTransformMat = localTransformMat;
	}
}

void ComponentTransform::PropagateTransform(GameObject* go, float3& newPos, Quat& newQuat, float3& newScale)
{
	RecomputeGlobalMat();

	parent->UpdateAABB(localTransformMat);

	std::vector<GameObject*>::iterator it = go->GetChilds().begin();
	for (; it != go->GetChilds().end(); ++it)
	{
		ComponentTransform* childTrans = (ComponentTransform*)(*it)->GetComponent(ComponentType::TRANSFORM);
		ComponentTransform* parentTrans = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);			

		if ((*it)->GetChilds().size() > 0)
			PropagateTransform((*it), newPos, newQuat, newScale);

		childTrans->SetTranslation(childTrans->GetTranslation() + newPos);
		childTrans->SetRotation(newQuat);
		childTrans->SetScale(newScale);
		(*it)->UpdateAABB(childTrans->GetLocalMatrix());

		childTrans->SetLocalMatrix(childTrans->localTransformMat);
	}

	ComponentCamera* cam = (ComponentCamera*)go->GetComponent(ComponentType::CAMERA);
	ComponentTransform* tr = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);
	if (cam)
	{
		cam->SetPosition(tr->GetTranslation());

		/*float3 r = rotation.ToEulerXYZ();
		r = math::RadToDeg(r);
		cam->Look(r);*/
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