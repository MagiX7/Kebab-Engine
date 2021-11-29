#include "PanelImportTexture.h"
#include "TextureLoader.h"
//#include "Texture.h"

#include "ResourceManager.h"

ImportTexturePanel::ImportTexturePanel()
{
	active = false;
}

ImportTexturePanel::~ImportTexturePanel()
{
}

void ImportTexturePanel::OnRender(float dt)
{
	static bool close = false;
	ImGui::Begin("Texture Import Settings", &close);

	static int compression = ImageCompression::DXTC_FORMAT;
	if (ImGui::Combo("Compression", &compression, "IL_DXTC_FORMAT\0IL_DXT1\0IL_DXT2\0IL_DXT3\0IL_DXT4\0IL_DXT5\0IL_DXT_NO_COMP\0", 5))
	{
		// TODO: Update current compression
	}

	ImGui::Separator();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Settings", flags))
	{
		// TODO: Implement mip mapping !!

		ImGui::TableNextColumn(); ImGui::Checkbox("Mip-Map", &props.mipmap);
		//if(props.mipmap)
		//	ImGui::TableNextColumn(); ImGui::Checkbox("Anystropy", &props.anystropy);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Filters", flags))
	{
		if (ImGui::BeginTable("Filters", 3))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Alienify", &props.alienify);
			ImGui::TableNextColumn(); ImGui::Checkbox("Gaussian Blur", &props.gaussianBlur);
			if (props.gaussianBlur)
				ImGui::SliderInt("Iterations", &props.gaussianBlurIterations, 1, 10);

			ImGui::TableNextColumn(); ImGui::Checkbox("Average Blur", &props.averageBlur);
			if(props.averageBlur)
				ImGui::SliderInt("Iterations", &props.gaussianBlurIterations, 1, 10);

			ImGui::TableNextColumn(); ImGui::Checkbox("Contrast", &props.contrast);
			if(props.contrast)
				ImGui::SliderFloat("Amount", &props.contrastAmount, -5.0f, 1.7f);

			ImGui::TableNextColumn(); ImGui::Checkbox("Equalization", &props.equalization);
			ImGui::TableNextColumn(); ImGui::Checkbox("Gamma Correction", &props.gammaCorrection);
			if (props.gammaCorrection)
				ImGui::SliderFloat("Amount", &props.gammaCorrectionAmount, 0.0f, 1.5f);

			ImGui::TableNextColumn(); ImGui::Checkbox("Negativity", &props.negativity);
			ImGui::TableNextColumn(); ImGui::Checkbox("Noise", &props.noise);
			if(props.noise)
				ImGui::SliderFloat("Amount", &props.noiseAmount, 0.0f, 1.0f);

			ImGui::TableNextColumn(); ImGui::Checkbox("Pixelization", &props.pixelization);
			if(props.pixelization)
				ImGui::SliderFloat("Pixels size", &props.pixelsSize, 1.0f, 50.0f);
			ImGui::TableNextColumn(); ImGui::Checkbox("Sharpening", &props.sharpening);
			if (props.sharpening)
			{
				ImGui::SliderFloat("Amount", &props.sharpeningAmount, 0.0f, 2.5f);
				ImGui::SliderInt("Iterations", &props.sharpeningIterations, 1, 10);
			}

			ImGui::EndTable();
		}
	}
	
	ImGui::Separator();

	if (ImGui::Button("Import", { 60,25 }))
	{
		// TODO: Import/Load image
		std::shared_ptr<Resource> texture = ResourceManager::GetInstance()->CreateTexture(assetsPath.c_str(), 0, props);
		TextureLoader::GetInstance()->SaveTextureCustomFormat((Texture*)texture.get(), 0);

		active = false;
	}

	ImGui::End();
}

void ImportTexturePanel::SetTexturePath(const char* assetsFile)
{
	assetsPath = assetsFile;
}