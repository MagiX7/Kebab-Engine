#include "Application.h"
#include "Renderer3D.h"

#include "ComponentLight.h"

#include "imgui/imgui.h"

ComponentLight::ComponentLight()
{
	this->type = ComponentType::LIGHT;
}

ComponentLight::~ComponentLight()
{
	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			delete app->renderer3D->dirLight;
			app->renderer3D->dirLight = 0;

			break;
		}

		case LightType::POINT:
		{
			app->renderer3D->DeletePointLight((PointLight*)light);
	
			break;
		}
	}
}

void ComponentLight::Update(float dt)
{
	if (light->type == LightType::POINT)
	{
		if (ComponentTransform* tr = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM))
		{
			PointLight* l = (PointLight*)light;
			if (tr->GetTranslation().x != l->position.x || tr->GetTranslation().y != l->position.y || tr->GetTranslation().z != l->position.z)
			{
				l->position = tr->GetTranslation();
				light = l;
			}
		}
	}
	else if (light->type == LightType::SPOT)
	{
		if (ComponentTransform* tr = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM))
		{
			SpotLight* l = (SpotLight*)light;
			if (tr->GetTranslation().x != l->position.x || tr->GetTranslation().y != l->position.y || tr->GetTranslation().z != l->position.z)
			{
				l->position = tr->GetTranslation();
				light = l;
			}
		}
	}
}

void ComponentLight::DrawOnInspector()
{
	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Separator();
				ImGui::Dummy({ 10,10 });

				DirectionalLight* l = (DirectionalLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());
			}

			break;
		}
		
		case LightType::POINT:
		{
			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				PointLight* l = (PointLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

				ImGui::DragFloat("Intensity", &l->intensity, 0.1f);

				ImGui::DragFloat("Linear", &l->lin, 0.001f);
				ImGui::DragFloat("Constant", &l->constant, 0.1f, 0.0f, 100.0f);
				ImGui::DragFloat("Quadratic", &l->quadratic, 0.001f, 0.0f, 1.0f);
			}

			break;
		}

		case LightType::SPOT:
		{
			SpotLight* l = (SpotLight*)light;

			ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
			ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
			ImGui::ColorEdit3("Specular Color", l->specular.ptr());

			ImGui::DragFloat("Intensity", &l->intensity, 0.1f);
			ImGui::DragFloat("CutOff", &l->cutOff, 0.1f);
			ImGui::DragFloat("Outer CutOff", &l->outerCutOff, 0.1f);
		}
	}
}

void ComponentLight::SetLight(Light* light)
{
	this->light = light;
}

JSON_Value* ComponentLight::Save()
{
	JSON_Value* value = json_value_init_object();
	JSON_Object* obj = json_value_get_object(value);

	json_object_set_number(obj, "Type", 5);
	
	if(light->type == LightType::DIRECTIONAL)
		json_object_set_string(obj, "lightType", "directional");
	else if (light->type == LightType::POINT)
		json_object_set_string(obj, "lightType", "point");

	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			DirectionalLight* l = (DirectionalLight*)light;
			
			json_object_dotset_number(obj, "dir.x", l->dir.x);
			json_object_dotset_number(obj, "dir.y", l->dir.y);
			json_object_dotset_number(obj, "dir.z", l->dir.z);

			json_object_dotset_number(obj, "ambient.x", l->ambient.x);
			json_object_dotset_number(obj, "ambient.y", l->ambient.y);
			json_object_dotset_number(obj, "ambient.z", l->ambient.z);
			
			json_object_dotset_number(obj, "diffuse.x", l->diffuse.x);
			json_object_dotset_number(obj, "diffuse.y", l->diffuse.y);
			json_object_dotset_number(obj, "diffuse.z", l->diffuse.z);

			json_object_dotset_number(obj, "specular.x", l->specular.x);
			json_object_dotset_number(obj, "specular.y", l->specular.y);
			json_object_dotset_number(obj, "specular.z", l->specular.z);

			break;
		}
		case LightType::POINT:
		{
			PointLight* l = (PointLight*)light;

			json_object_dotset_number(obj, "pos.x", l->position.x);
			json_object_dotset_number(obj, "pos.y", l->position.y);
			json_object_dotset_number(obj, "pos.z", l->position.z);

			json_object_dotset_number(obj, "ambient.x", l->ambient.x);
			json_object_dotset_number(obj, "ambient.y", l->ambient.y);
			json_object_dotset_number(obj, "ambient.z", l->ambient.z);

			json_object_dotset_number(obj, "diffuse.x", l->diffuse.x);
			json_object_dotset_number(obj, "diffuse.y", l->diffuse.y);
			json_object_dotset_number(obj, "diffuse.z", l->diffuse.z);

			json_object_dotset_number(obj, "specular.x", l->specular.x);
			json_object_dotset_number(obj, "specular.y", l->specular.y);
			json_object_dotset_number(obj, "specular.z", l->specular.z);

			json_object_set_number(obj, "constant", l->constant);
			json_object_set_number(obj, "linear", l->lin);
			json_object_set_number(obj, "quadratic", l->quadratic);


			break;
		}
	}

	return value;
}

void ComponentLight::Load(JSON_Object* obj, GameObject* parent)
{
	light = new Light();

	std::string lightType = json_object_get_string(obj, "lightType");
	if (lightType == "directional")
	{
		light->type = LightType::DIRECTIONAL;
		light = app->renderer3D->dirLight;
		return;
	}
	else if (lightType == "point")
		light->type = LightType::POINT;


	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			DirectionalLight* l = (DirectionalLight*)light;

			l->dir.x = json_object_dotget_number(obj, "dir.x");
			l->dir.y = json_object_dotget_number(obj, "dir.y");
			l->dir.z = json_object_dotget_number(obj, "dir.z");

			l->ambient.x = json_object_dotget_number(obj, "ambient.x");
			l->ambient.y = json_object_dotget_number(obj, "ambient.y");
			l->ambient.z = json_object_dotget_number(obj, "ambient.z");

			l->diffuse.x = json_object_dotget_number(obj, "diffuse.x");
			l->diffuse.y = json_object_dotget_number(obj, "diffuse.y");
			l->diffuse.z = json_object_dotget_number(obj, "diffuse.z");

			l->specular.x = json_object_dotget_number(obj, "specular.x");
			l->specular.y = json_object_dotget_number(obj, "specular.y");
			l->specular.z = json_object_dotget_number(obj, "specular.z");

			light = l;

			break;
		}
		case LightType::POINT:
		{
			PointLight* l = (PointLight*)light;

			l->position.x = json_object_dotget_number(obj, "position.x");
			l->position.y = json_object_dotget_number(obj, "position.y");
			l->position.z = json_object_dotget_number(obj, "position.z");

			l->ambient.x = json_object_dotget_number(obj, "ambient.x");
			l->ambient.y = json_object_dotget_number(obj, "ambient.y");
			l->ambient.z = json_object_dotget_number(obj, "ambient.z");

			l->diffuse.x = json_object_dotget_number(obj, "diffuse.x");
			l->diffuse.y = json_object_dotget_number(obj, "diffuse.y");
			l->diffuse.z = json_object_dotget_number(obj, "diffuse.z");

			l->specular.x = json_object_dotget_number(obj, "specular.x");
			l->specular.y = json_object_dotget_number(obj, "specular.y");
			l->specular.z = json_object_dotget_number(obj, "specular.z");

			l->constant = json_object_get_number(obj, "constant");
			l->lin = json_object_get_number(obj, "linear");
			l->quadratic = json_object_get_number(obj, "quadratic");

			light = l;

			break;
		}
	}
}
