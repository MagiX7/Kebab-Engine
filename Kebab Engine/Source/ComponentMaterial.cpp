#include "ComponentMaterial.h"

#include "TextureLoader.h"
#include "ResourceManager.h"

#include "imgui/imgui.h"

#define CHECKERS_HEIGHT 40
#define CHECKERS_WIDTH 40

ComponentMaterial::ComponentMaterial(GameObject* compOwner)
{
	this->parent = compOwner;
	this->type = ComponentType::MATERIAL;
	this->active = true;

	SetCheckersTexture();
}

ComponentMaterial::~ComponentMaterial()
{
	/*for (auto& t : textures)
	{
		delete t;
		t = nullptr;
	}*/
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
		if (currentTexture)
		{
			std::string s = "References to texture " + std::to_string(ResourceManager::GetInstance()->GetReferenceCount(currentTexture->uuid));
			ImGui::BulletText(s.c_str());
		}
	}
}

void ComponentMaterial::AddTexture(Texture* tex)
{
	//std::vector<Texture*>::iterator it = std::find(textures.begin(), textures.end(), tex);
	if (std::find(textures.begin(), textures.end(), tex) == textures.end())
	{
		textures.push_back(tex);
		texture = tex;
		//texture->SetUUID(tex->uuid);
		currentTexture = tex;
	}
}

JSON_Value* ComponentMaterial::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	json_object_set_number(obj, "Type", 2);

	if (currentTexture == checkersTexture) json_object_set_string(obj, "path", "Checkers");
	if (currentTexture == texture) json_object_set_string(obj, "path", texture->GetPath().c_str());

	return value;
}

void ComponentMaterial::Load(JSON_Object* obj, GameObject* parent)
{
	const char* texName = json_object_dotget_string(obj, "path");
	if (texName == "Checkers") currentTexture = checkersTexture;
	else
	{
		if (texture) delete texture;
		texture = TextureLoader::GetInstance()->LoadTextureCustomFormat(texName);
		currentTexture = texture;
	}

	//this->parent = parent;
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