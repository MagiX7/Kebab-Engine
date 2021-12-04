#include "PanelImportModel.h"

#include "ResourceManager.h"
#include "MeshLoader.h"

#include "Model.h"

PanelImportModel::PanelImportModel()
{
	active = false;
}

PanelImportModel::~PanelImportModel()
{
}

void PanelImportModel::OnRender(float dt)
{
	ImGui::Begin("Model Import Options", &active);

	//joinIdenticalVertices = false;
	//triangulate = true;
	//genNormals = false;
	//genSmoothNormals = false;
	//removeRedundantMaterials = false;
	//genUVCoords = true;
	//transformUVCoords = false;
	//findInstances = true;
	//optimizeMesh = true;

	ImGui::TableNextColumn();
	ImGui::Checkbox("Join Identical Vertices", &props.joinIdenticalVertices);	
	HelpMarker("Joins the identical vertices into one (Recommended activation).");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Triangulate", &props.triangulate);
	HelpMarker("Converts all the polys to triangles.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Generate Normals", &props.genNormals);
	HelpMarker("Generates the normals of the model.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Generate Smooth Normals", &props.genSmoothNormals);
	HelpMarker("Generates smooth normals for all vertices in the mesh.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Remove Redundat Materials", &props.removeRedundantMaterials);
	HelpMarker("Searches for redundant/unreferenced materials and removes them.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Fix Infacing Normals", &props.fixInfacingNormals);
	HelpMarker("A try to determine which meshes have normal vectors that are facing inwards and inverts them.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Generate UV Coordinates", &props.genUVCoords);
	HelpMarker("Convert non-UV mappings (such as spherical or cylindrical mapping) to proper texture coordinate channels.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Transform UV Coordinates", &props.transformUVCoords);
	HelpMarker("Applies per-texture UV transformations and bakes them into stand-alone vtexture coordinate channels.");
	
	ImGui::TableNextColumn();
	ImGui::Checkbox("Find Instances", &props.findInstances);
	HelpMarker("Search for duplicate meshes and replaces them with references to the first mesh.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Optimize Mesh", &props.optimizeMeshes);
	HelpMarker("Reduce the number of meshes.");

	ImGui::TableNextColumn();
	ImGui::Checkbox("Flip UVs", &props.flipUVs);
	HelpMarker("Flips all UV coordinates along the y-axis and adjusts material settings and bitangents accordingly.");

	if (ImGui::Button("Import"))
	{
		MeshLoader::GetInstance()->ReLoadModel(assetsPath.c_str(), props);

		//std::shared_ptr<KbModel> model = std::static_pointer_cast<KbModel>(ResourceManager::GetInstance()->IsAlreadyLoaded(assetsPath.c_str()));

		////ResourceManager::GetInstance()->CreateNewResource(assetsPath.c_str(), ResourceType::MODEL);
		//if (model)
		//{
		//	model->SetProperties(props);
		//}
		//else
		//{
		//	model = std::static_pointer_cast<KbModel>(ResourceManager::GetInstance()->CreateNewResource(assetsPath.c_str(), ResourceType::MODEL));
		//	//MeshLoader::GetInstance()->LoadModel(assetsPath.c_str());
		//}

		active = false;
	}

	ImGui::End();
}

void PanelImportModel::HelpMarker(const char* text)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}