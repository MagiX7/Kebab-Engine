#include "Application.h"

#include "Input.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "FileSystem.h"
#include "MainScene.h"

#include "PanelScene.h"

#include "ResourceManager.h"

#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "PanelViewport.h"
#include "PanelHierarchy.h"

#include "TextureLoader.h"

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
        app->camera->editorCam->CalculateFov(viewportPanelSize.x, viewportPanelSize.y);
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
            name = name.substr(0, name.find_last_of("_"));
            name = name.substr(0, name.find_last_of("_"));

            if (ext == ".kbmodel")
            {
                std::string pathAsset = app->fileSystem->FindFilePath(name + ".fbx");

                if (pathAsset != "")
                {
                    GameObject* bh = MeshLoader::GetInstance()->LoadModel(pathAsset, true);
                    app->renderer3D->Submit(bh);
                }
                else
                {
                    pathAsset = app->fileSystem->FindFilePath(name + ".obj");

                    if (pathAsset != "")
                    {
                        GameObject* bh = MeshLoader::GetInstance()->LoadModel(pathAsset, true);
                        app->renderer3D->Submit(bh);
                    }
                }
            }
            else if (ext == ".kbtexture")
            {
                if (app->editor->hierarchyPanel->currentGO)
                {
                    GameObject* target = app->editor->hierarchyPanel->currentGO;

                    if (target->GetComponent(ComponentType::MESH))
                    {
                        for (int i = 0; i < target->GetComponents().size(); ++i)
                        {
                            ComponentMesh* mesh = (ComponentMesh*)target->GetComponent(ComponentType::MESH);
                            GameObject* parent = target->GetParent();
                            
                            ComponentMaterial* mat = (ComponentMaterial*)target->GetComponent(ComponentType::MATERIAL);
                            std::shared_ptr<Resource> tex = ResourceManager::GetInstance()->IsAlreadyLoaded(dragPath);
                            mat->AddTexture(std::static_pointer_cast<Texture>(tex));

                            //mat->AddTexture(TextureLoader::GetInstance()->LoadTextureCustomFormat(dragPath));
                        }
                    }
                    else
                    {
                        std::string message = "Couldn't apply texture, selected game object "
                            + target->GetName()
                            + " doesn't have a mesh. Try with a child game object that has a mesh instead.";
                        LOG_CONSOLE(message.c_str());
                    }
                }
            }
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
            ComponentCamera* cam = app->camera->editorCam;

            float4x4 model = float4x4::identity;

            if (go->GetParent()->GetParent() != nullptr)
                model = tr->GetGlobalMatrix();
            else
                model = tr->GetLocalMatrix();

            model.Transpose();

            ImGuizmo::Enable(true);
            ImGuizmo::SetRect(viewportDimensions.x, viewportDimensions.y, viewportDimensions.z, viewportDimensions.w);

            ImGuizmo::SetDrawlist();

            ImGuizmo::Manipulate(cam->GetViewMatrix().ptr(), cam->GetProjectionMatrix().ptr(), op, mode, model.ptr());

            if (ImGuizmo::IsUsing())
            {
                model.Transpose();

                if (go->GetParent()->GetParent() != nullptr)
                    tr->SetGlobalMatrix(model);
                else
                    tr->SetLocalMatrix(model);

                go->HasMoved();
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
