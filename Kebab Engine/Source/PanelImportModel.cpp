#include "Application.h"
#include "Renderer3D.h"
#include "FileSystem.h"

#include "PanelImportModel.h"

#include "ResourceManager.h"
#include "MeshLoader.h"

#include "Model.h"

#include "mmgr/mmgr.h"

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
		if (ResourceManager::GetInstance()->IsAlreadyLoaded(assetsPath.c_str()))
		{
			MeshLoader::GetInstance()->ReLoadModel(assetsPath.c_str(), props);
		}
		else
		{
			app->renderer3D->Submit(MeshLoader::GetInstance()->LoadModel(assetsPath, true));
			model = std::static_pointer_cast<KbModel>(ResourceManager::GetInstance()->IsAlreadyLoaded(assetsPath.c_str()));
			model->SetAssetsPath(assetsPath);

			if (assetsPath.find("Assets/"))
			{
				std::string metaPath = assetsPath + ".meta";
				app->fileSystem->Remove(metaPath.c_str());

				int s = assetsPath.find_last_of("/");
				if(s == -1)
					s = assetsPath.find_last_of("\\");

				int e = assetsPath.find_last_of(".");
				std::string n = assetsPath.substr(s + 1, e - s - 1);
				std::string newMetaPath = "Assets/Resources/" + n + extension;

				model->CreateMetaDataFile(newMetaPath.c_str());
			}

		}

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

void PanelImportModel::SetAssetsPath(const std::string& path)
{
	if (!path.empty())
	{
		assetsPath = path;
		
		extension = path.substr(path.find("."));
	}
}
