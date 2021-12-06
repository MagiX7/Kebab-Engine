#include "Application.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "MainScene.h"
//#include "ComponentCamera.h"

#include "PanelScene.h"

#include "Buffer.h"

#include "mmgr/mmgr.h"

ScenePanel::ScenePanel()
{
    dimensions = { 0,0,0,0 };
    size = { 0,0 };
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::OnRender(FrameBuffer* frameBuffer)
{
    ImGui::Begin("Game");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (size.x != viewportPanelSize.x || size.y != viewportPanelSize.y)
    {
        frameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
        size = { viewportPanelSize.x, viewportPanelSize.y };
        app->renderer3D->OnResize(viewportPanelSize.x, viewportPanelSize.y);
        if (app->camera->gameCam)
            app->camera->gameCam->CalculateFov(viewportPanelSize.x, viewportPanelSize.y);
    }

    dimensions = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowWidth(),ImGui::GetWindowHeight() };

    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { viewportPanelSize.x, viewportPanelSize.y }, { 0,1 }, { 1,0 });

    ImGui::End();
}