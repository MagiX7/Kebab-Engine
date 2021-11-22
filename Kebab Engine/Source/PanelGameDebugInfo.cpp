#include "Application.h"
#include "PanelGameDebugInfo.h"

GameDebugInfoPanel::GameDebugInfoPanel()
{

}

GameDebugInfoPanel::~GameDebugInfoPanel()
{

}

void GameDebugInfoPanel::OnRender(float dt)
{
	if (ImGui::Begin("Game Debug Info"))
	{
		ImGui::BulletText("Current dt: %f", app->GetRuntimeDt());
		ImGui::BulletText("Current FPS: %f", app->GetRuntimeDt() == 0 ? 0 : 1 / app->GetRuntimeDt());
		ImGui::BulletText("Amount of frames: %i", app->GetRuntimeFramesAmount());

		ImGui::End();
	}
}