#include "Application.h"

#include "Input.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "Editor.h"

#include "ComponentCamera.h"

#include "PanelViewport.h"
#include "PanelHierarchy.h"

ViewportPanel::ViewportPanel()
{
    viewportDimensions = { 0,0,0,0 };
    viewportSize = { 0,0 };
    hovered = false;
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::OnRender(FrameBuffer* frameBuffer, const ImGuizmo::OPERATION& op, const ImGuizmo::MODE& mode)
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

    DrawGuizmo(op, mode);

    ImGui::End();
}

float4 ViewportPanel::GetDimensions()
{
    return viewportDimensions;
}

bool ViewportPanel::IsHovered()
{
    return hovered;
}

void ViewportPanel::DrawGuizmo(const ImGuizmo::OPERATION& op, const ImGuizmo::MODE& mode)
{
    GameObject* go = app->editor->hierarchyPanel->currentGO;
    if (go)
    {
        ComponentTransform* tr = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);

        ComponentCamera* cam = app->camera->GetCamera();

        float4x4 model = tr->GetLocalMatrix();
        model.Transpose();

        ImGuizmo::Enable(true);
        ImGuizmo::SetRect(viewportDimensions.x, viewportDimensions.y, viewportDimensions.z, viewportDimensions.w);

        ImGuizmo::SetDrawlist();

        ImGuizmo::Manipulate(cam->GetViewMatrix().ptr(), cam->GetProjectionMatrix().ptr(), op, mode, model.ptr());

        if (ImGuizmo::IsUsing())
        {
            model.Transpose();
            tr->SetLocalMatrix(model);
        }
    }
}
