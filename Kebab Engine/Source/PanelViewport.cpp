#include "Application.h"
#include "PanelViewport.h"

ViewportPanel::ViewportPanel()
{
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::OnRender(FrameBuffer* frameBuffer)
{
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    if (viewportSize.x != viewportPanelSize.x || viewportSize.y != viewportPanelSize.y)
    {
        frameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
        viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        app->renderer3D->OnResize(viewportPanelSize.x, viewportPanelSize.y);
    }
    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { viewportPanelSize.x, viewportPanelSize.y }, { 0,1 }, { 1,0 });
    ImGui::End();
}