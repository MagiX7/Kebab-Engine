#include "Application.h"
#include "Renderer3D.h"

#include "PanelLightSettings.h"

PanelLightSettings::PanelLightSettings()
{
	active = true;
}

PanelLightSettings::~PanelLightSettings()
{
}

void PanelLightSettings::OnRender(float dt)
{
	ImGui::Begin("Lightning Settings", &active);
	
	ImGui::TextColored({ 255,255,0,255 }, "Global Settings");

	ImGui::Checkbox("Enable Gamma Correction", &app->renderer3D->gammaCorrection);
	if (app->renderer3D->gammaCorrection)
	{
		ImGui::DragFloat("Gamma Correction Amount", &app->renderer3D->gammaCorrectionAmount, 0.01f);
	}

	ImGui::Separator();

	ImGui::End();
}
