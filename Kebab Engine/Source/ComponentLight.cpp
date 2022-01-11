#include "ComponentLight.h"

#include "imgui/imgui.h"

ComponentLight::ComponentLight()
{

}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::DrawOnInspector()
{
	switch (light->type)
	{
		case LightType::DIRECTIONAL:
		{
			if (ImGui::CollapsingHeader("Light"))
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
			if (ImGui::CollapsingHeader("Light"))
			{
				PointLight* l = (PointLight*)light;

				ImGui::ColorEdit3("Ambient Color", l->ambient.ptr());
				ImGui::ColorEdit3("Diffuse Color", l->diffuse.ptr());
				ImGui::ColorEdit3("Specular Color", l->specular.ptr());

			}

			break;
		}
	}
}

void ComponentLight::SetLight(Light* light)
{
	this->light = light;
}
