#include "Application.h"

#include "Input.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "FileSystem.h"

#include "PanelScene.h"

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

    /*if (ImGui::IsItemActive() && app->camera->GetCurrentCamera() != app->camera->editorCam)
        app->camera->SetCurrentCamera(app->camera->editorCam);*/

    if (viewportSize.x != viewportPanelSize.x || viewportSize.y != viewportPanelSize.y)
    {
        frameBuffer->Resize(viewportPanelSize.x, viewportPanelSize.y);
        viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        app->renderer3D->OnResize(viewportPanelSize.x, viewportPanelSize.y);
    }

    viewportDimensions = { ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,ImGui::GetWindowWidth(),ImGui::GetWindowHeight() };

    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { viewportPanelSize.x, viewportPanelSize.y }, { 0,1 }, { 1,0 });

    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_ITEM");

        if (payload)
        {
            std::string dragPath = (const char*)payload->Data;
            std::string name = dragPath.substr(dragPath.find_last_of("/") + 1, dragPath.size());
            std::string ext = name.substr(name.find_last_of("."), name.size());
            name = name.substr(0, name.find_last_of("."));

            bool isCustom = false;

            
            if (ext == ".fbx" || ext == ".obj")
            {
                isCustom = true;

                name += ".kbmodel";

                GameObject* bh = MeshLoader::GetInstance()->LoadModelCustomFormat(name);
                if (bh)
                    app->renderer3D->Submit(bh);
                else
                {
                    isCustom = false;
                    name = name.substr(0, name.find_last_of("."));
                }
            }
            if (!isCustom && (ext == ".fbx" || ext == ".obj"))
            {
                GameObject* bh = MeshLoader::GetInstance()->LoadModel(dragPath);
                app->renderer3D->Submit(bh);
            }
            /*if (strcmp(ext.c_str(), ".dds") == 0 || strcmp(ext.c_str(), ".png") == 0 || strcmp(ext.c_str(), ".jpg") == 0)
                ext = ".kbtexture";*/
        }

        ImGui::EndDragDropTarget();
    }

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

        if (tr)
        {
            ComponentCamera* cam = app->camera->GetCurrentCamera();

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
}


////////////////////////////////////////////////
/////////////// ScenePreview ///////////////////
////////////////////////////////////////////////

ScenePreviewPanel::ScenePreviewPanel()
{
    dimensions = { 0,0,0,0 };
    size = { 0,0 };
}

ScenePreviewPanel::~ScenePreviewPanel()
{
}

void ScenePreviewPanel::OnRender(FrameBuffer* frameBuffer)
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

    bool close = false;
    ImGui::Begin("Scene Preview", &close, flags);

    float4 sd = app->editor->scenePanel->GetDimensions();
    
    if (size.x != sd.z * 0.4f || size.y != sd.w * 0.4f)
        size = { sd.z * 0.4f, sd.w * 0.4f };


    ImVec2 s = ImGui::GetWindowSize();
    float4 vd = app->editor->viewportPanel->GetDimensions();
    float2 p = { vd.x + vd.z - s.x - 5, vd.y + vd.w - s.y - 5 };

    uint32_t image = frameBuffer->GetColorAttachment();
    ImGui::Image((void*)image, { size.x, size.y }, { 0,1 }, { 1,0 });
    ImGui::End();
}

void ScenePreviewPanel::SetSize(const float2& newSize)
{
    size = newSize;
}
