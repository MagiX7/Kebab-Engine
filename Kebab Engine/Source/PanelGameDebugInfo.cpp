#include "Application.h"
#include "PanelGameDebugInfo.h"

#include "mmgr/mmgr.h"

GameDebugInfoPanel::GameDebugInfoPanel()
{

}

GameDebugInfoPanel::~GameDebugInfoPanel()
{

}

void GameDebugInfoPanel::OnRender(float dt)
{
	ImGui::Begin("Game Debug Info");
	
	ImGui::BulletText("Current dt: %f", app->GetRuntimeDt());
	ImGui::BulletText("Last dt: %f", app->GetLastRuntimeDt());
	ImGui::BulletText("Current FPS: %f", app->GetRuntimeDt() == 0 ? 0 : 1 / app->GetRuntimeDt());
	ImGui::BulletText("Amount of frames: %i", app->GetRuntimeFramesAmount());
	ImGui::BulletText("Total amount of frames: %i", app->GetToalRuntimeFramesAmount());

	ImGui::End();
	
}