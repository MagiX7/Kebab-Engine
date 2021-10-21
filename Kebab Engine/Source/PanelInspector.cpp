#include "Application.h"

#include "PanelInspector.h"

InspectorPanel::InspectorPanel()
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::OnRender(float dt)
{
	if (ImGui::Begin("Inspector"))
	{
		if (app->editor->hierarchyPanel->currentGO)
		{
			DisplayTransform(app->editor->hierarchyPanel->currentGO);
			//ImGui::Separator();
			DisplayMeshInfo(app->editor->hierarchyPanel->currentGO);
			DisplayTextureInfo(app->editor->hierarchyPanel->currentGO);
		}

		ImGui::End();
	}
}

void InspectorPanel::DisplayTransform(GameObject* go)
{
	ComponentTransform* transform = (ComponentTransform*)go->GetComponent(ComponentType::TRANSFORM);
	float3 p = transform->GetPosition();
	Quat r = transform->GetRotation();
	float3 s = transform->GetScale();

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		ImGui::TextWrapped("x: %.2f	y: %.2f	z: %.2f", p.x, p.y, p.z);

		ImGui::Separator();

		ImGui::Text("Rotation");
		ImGui::TextWrapped("x: %.2f	y: %.2f	z: %.2f", r.x, r.y, r.z);

		ImGui::Separator();

		ImGui::Text("Scale");
		ImGui::TextWrapped("x: %.2f	y: %.2f	z: %.2f", s.x, s.y, s.z);

		//ImGui::End();
	}
}

void InspectorPanel::DisplayMeshInfo(GameObject* go)
{
	ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);

	if (mesh)
	{
		int verticesCount = mesh->vertices.size();
		int indicesCount = mesh->indices.size();
		int texCount = mesh->textures.size();

		if (ImGui::CollapsingHeader("Mesh"))
		{
			ImGui::Text("Vertices: %i", verticesCount);
			ImGui::Text("Indices: %i", indicesCount);
			ImGui::Text("Textures: %i", texCount);
		}
	}

}

void InspectorPanel::DisplayTextureInfo(GameObject* go)
{
	ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);
	if (mesh)
	{
		if (ImGui::CollapsingHeader("Texture"))
		{
			ImGui::TextColored({ 255,255,0,255 }, "Path: ");
		}
	}
}