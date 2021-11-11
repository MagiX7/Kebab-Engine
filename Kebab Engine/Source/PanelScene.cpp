#include "Application.h"
#include "Renderer3D.h"

//#include "ComponentCamera.h"

#include "PanelScene.h"

#include "Buffer.h"

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
    ImGui::Begin("Scene");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    //hovered = ImGui::IsWindowHovered();

    if (size.x != viewportPanelSize.x || size.y != viewportPanelSize.y)
    {
        frameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
        size = { viewportPanelSize.x, viewportPanelSize.y };
        app->renderer3D->OnResize(viewportPanelSize.x, viewportPanelSize.y);
    }

    dimensions = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowWidth(),ImGui::GetWindowHeight() };

    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { viewportPanelSize.x, viewportPanelSize.y }, { 0,1 }, { 1,0 });

    ImGui::End();
}
