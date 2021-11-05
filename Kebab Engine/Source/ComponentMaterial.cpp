#include "ComponentMaterial.h"
#include "Texture.h"

#include "imgui/imgui.h"

#define CHECKERS_HEIGHT 40
#define CHECKERS_WIDTH 40

ComponentMaterial::ComponentMaterial(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::MATERIAL;
	this->active = true;

	SetCheckersTexture();
}

ComponentMaterial::~ComponentMaterial()
{
	for (auto& t : textures)
	{
		delete t;
		t = nullptr;
	}
	textures.clear();

	delete checkersTexture;
}

void ComponentMaterial::Bind()
{
	if(currentTexture) currentTexture->Bind();
}

void ComponentMaterial::Unbind()
{
	if (currentTexture) currentTexture->Unbind();
}

void ComponentMaterial::Enable()
{
	this->active = true;
}

void ComponentMaterial::Disable()
{
	this->active = false;
}

void ComponentMaterial::DrawOnInspector()
{
	if (ImGui::CollapsingHeader("Texture"))
	{
		if (currentTexture && currentTexture == texture)
			ImGui::TextColored({ 255,255,0,255 }, "Path: %s", texture->GetPath().c_str());
		else if (currentTexture == checkersTexture)
			ImGui::TextColored({ 255,255,0,255 }, "Default checkers texture");
		else
			ImGui::TextColored({ 255,255,0,255 }, "Path: This mesh does not have a texture");

		static bool checkers = currentTexture;
		if (currentTexture == checkersTexture) checkers = true;
		else checkers = false;

		if (ImGui::Checkbox("Set checkers", &checkers))
		{
			if (checkers) currentTexture = checkersTexture;
			else if (texture) currentTexture = texture;
			else currentTexture = nullptr;
		}

		ImGui::NewLine();
		ImGui::BulletText("Current Texture: ");
		if (currentTexture)
			ImGui::Image((void*)currentTexture->GetID(), { 150,150 });
		else
		{
			ImGui::SameLine();
			ImGui::Text("No texture.");
		}
	}
}

void ComponentMaterial::AddTexture(Texture* tex)
{
	std::vector<Texture*>::iterator it = std::find(textures.begin(), textures.end(), tex);
	if (it == textures.end())
	{
		textures.push_back(tex);
		texture = tex;
		currentTexture = tex;
	}
}

void ComponentMaterial::SetCheckersTexture()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 225;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)225;
		}
	}
	checkersTexture = new Texture(checkerImage, CHECKERS_WIDTH, CHECKERS_HEIGHT, "Checkers");
	currentTexture = checkersTexture;
	//return checkersTexture;
}