#include "Application.h"
#include "PanelViewport.h"

#include "Renderer3D.h"

ViewportPanel::ViewportPanel()
{
    viewportDimensions = { 0,0,0,0 };
    viewportSize = { 0,0 };
    hovered = false;
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::OnRender(FrameBuffer* frameBuffer)
{
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    hovered = ImGui::IsWindowHovered();

    if (viewportSize.x != viewportPanelSize.x || viewportSize.y != viewportPanelSize.y)
    {
        frameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
        viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        app->renderer3D->OnResize(viewportPanelSize.x, viewportPanelSize.y);
    }

    viewportDimensions = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowWidth(),ImGui::GetWindowHeight() };

    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { viewportPanelSize.x, viewportPanelSize.y }, { 0,1 }, { 1,0 });
    ImGui::End();
}

float4 ViewportPanel::GetViewportDimensions()
{
    return viewportDimensions;
}

bool ViewportPanel::IsHovered()
{
    return hovered;
}
