#include "PanelImportTexture.h"
#include "Texture.h"

ImportTexturePanel::ImportTexturePanel()
{
	active = false;
}

ImportTexturePanel::~ImportTexturePanel()
{
}

void ImportTexturePanel::OnRender(float dt)
{
	ImGui::Begin("Texture Import Settings");

	static int compression = ImageCompression::DXTC_FORMAT;
	if (ImGui::Combo("Compression", &compression, "IL_DXTC_FORMAT\0IL_DXT1\0IL_DXT2\0IL_DXT3\0IL_DXT4\0IL_DXT5\0IL_DXT_NO_COMP\0", 5))
	{
		// TODO: Update current compression. Maybe a var in Texture?
	}
	
	ImGui::Separator();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Settings", flags))
	{
		ImGui::TableNextColumn(); ImGui::Checkbox("Mip-Map", &mipmap);
		ImGui::TableNextColumn(); ImGui::Checkbox("Anystropy", &anystropy);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Filters", flags))
	{
		if (ImGui::BeginTable("Filters", 3))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Gaussian Blur", &gaussianBlur);
			ImGui::TableNextColumn(); ImGui::Checkbox("Average Blur", &averageBlur);
			ImGui::TableNextColumn(); ImGui::Checkbox("Contrast", &contrast);
			ImGui::TableNextColumn(); ImGui::Checkbox("Alienify", &alienify);
			ImGui::TableNextColumn(); ImGui::Checkbox("Equalization", &equalization);
			ImGui::TableNextColumn(); ImGui::Checkbox("Gamma Correction", &gammaCorrection);
			ImGui::TableNextColumn(); ImGui::Checkbox("Negativity", &negativity);
			ImGui::TableNextColumn(); ImGui::Checkbox("Noise", &noise);
			ImGui::TableNextColumn(); ImGui::Checkbox("Pixelization", &pixelization);
			ImGui::TableNextColumn(); ImGui::Checkbox("Sharpening", &sharpening);
			
			ImGui::EndTable();
		}
	}
	
	ImGui::Separator();

	if (ImGui::Button("Import", { 60,25 }))
	{
		// TODO: Import/Load image
		active = false;
	}


	ImGui::End();

}
