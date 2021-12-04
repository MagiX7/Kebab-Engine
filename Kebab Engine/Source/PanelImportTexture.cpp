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

	const char* options[] = { "IL_DXT1\0IL_DXT2\0IL_DXT3\0IL_DXT4\0IL_DXT5\0IL_DXT_NO_COMP\0" };
	static int index = 4;
	if (ImGui::Combo("Compression", &index, "IL_DXT1\0IL_DXT2\0IL_DXT3\0IL_DXT4\0IL_DXT5\0IL_DXT_NO_COMP\0", 6))
	{
		// TODO: Update current compression
		for (int i = 0; i < IM_ARRAYSIZE(options); ++i)
		{
			const bool is_selected = (index == i);
			if (ImGui::Selectable(options[i], is_selected))
				index = i;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		//ImGui::EndCombo();
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
				if (ImGui::SliderFloat("Amount", &props.contrastAmount, -1.0f, 2.f))
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
				texture->ReLoad(props);
				change = false;
			}
		}
	}
	
	ImGui::Separator();

	if (ImGui::Button("Import", { 60,25 }))
	{
		props.compression = GetCompressionFromType(index);
		texture->ReLoad(props, true);
		active = false;
	}
	ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 70);
	if (ImGui::Button("Restart", { 60,25 }))
	{
		props = initialProps;
		texture->ReLoad(initialProps);
	}
	ImGui::Dummy({ ImGui::GetWindowContentRegionMax().x - 117,25 });
	ImGui::SameLine();
	if (ImGui::Button("Set Default"))
	{
		props = TextureProperties();
		texture->ReLoad(props);
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

ILint ImportTexturePanel::GetCompressionFromType(int compression)
{
	switch (compression)
	{
		case 0: return IL_DXT1;
		case 1: return IL_DXT2;
		case 2: return IL_DXT3;
		case 3: return IL_DXT4;
		case 4: return IL_DXT5;
		case 5: return IL_DXT_NO_COMP;
		default: return -1;
	}
}