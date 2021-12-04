#include "PanelImportTexture.h"
#include "TextureLoader.h"
#include "Texture.h"

#include "ResourceManager.h"

ImportTexturePanel::ImportTexturePanel()
{
	active = false;
	texture = nullptr;
}

ImportTexturePanel::~ImportTexturePanel()
{
}

void ImportTexturePanel::OnRender(float dt)
{
	ImGui::Begin("Texture Import Settings", &active);

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
			static bool change = false;

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("Alienify", &props.alienify))
				change = true;

			ImGui::TableNextColumn();
			ImGui::Checkbox("Gaussian Blur", &props.gaussianBlur);
			if (props.gaussianBlur)
				if (ImGui::SliderInt("Iterations", &props.gaussianBlurIterations, 1, 10))
					change = true;

			ImGui::TableNextColumn(); ImGui::Checkbox("Average Blur", &props.averageBlur);
			if (props.averageBlur)
				if (ImGui::SliderInt("Iterations", &props.gaussianBlurIterations, 1, 10))
					change = true;

			ImGui::TableNextColumn(); ImGui::Checkbox("Contrast", &props.contrast);
			if (props.contrast)
				if (ImGui::SliderFloat("Amount", &props.contrastAmount, -5.0f, 1.7f))
					change = true;

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("Equalization", &props.equalization))
				change = true;

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("Gamma Correction", &props.gammaCorrection));
			if (props.gammaCorrection)
				if (ImGui::SliderFloat("Amount", &props.gammaCorrectionAmount, 0.0f, 1.5f))
					change = true;

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("Negativity", &props.negativity))
				change = true;

			ImGui::TableNextColumn();
			ImGui::Checkbox("Noise", &props.noise);
			if (props.noise)
				if (ImGui::SliderFloat("Amount", &props.noiseAmount, 0.0f, 1.0f))
					change = true;

			ImGui::TableNextColumn();
			ImGui::Checkbox("Pixelization", &props.pixelization);
			if (props.pixelization)
				if (ImGui::SliderFloat("Pixels size", &props.pixelsSize, 1.0f, 50.0f))
					change = true;

			ImGui::TableNextColumn(); ImGui::Checkbox("Sharpening", &props.sharpening);
			if (props.sharpening)
			{
				if (ImGui::SliderFloat("Amount", &props.sharpeningAmount, 0.0f, 2.5f))
					change = true;
				if (ImGui::SliderInt("Iterations", &props.sharpeningIterations, 1, 10))
					change = true;
			}

			ImGui::EndTable();

			if (change)
			{
				texture->Reimport(props);
				change = false;
			}
		}
	}
	
	ImGui::Separator();

	if (ImGui::Button("Import", { 60,25 }))
	{
		texture->Reimport(props);
		active = false;
	}

	if (texture) ImGui::Image((void*)texture->GetID(), { 200,200 });

	ImGui::End();
}

void ImportTexturePanel::SetTexturePath(const char* assetsFile)
{
	assetsPath = assetsFile;
	texture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->IsAlreadyLoaded(assetsFile));

	//texture = TextureLoader::GetInstance()->LoadTexture(assetsFile);
}