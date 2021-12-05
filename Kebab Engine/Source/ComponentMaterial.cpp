#include "Application.h"
#include "ComponentMaterial.h"

#include "ResourceManager.h"
#include "Editor.h"

#include "PanelAssets.h"

#include "imgui/imgui.h"

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT 40
#define CHECKERS_WIDTH 40

ComponentMaterial::ComponentMaterial(GameObject* compOwner)
{
	this->parent = compOwner;
	this->type = ComponentType::MATERIAL;
	this->active = true;

	texture = nullptr;
	currentTexture = nullptr;
	checkersTexture = nullptr;

	menuSelectTex = false;

	SetCheckersTexture();
}

ComponentMaterial::~ComponentMaterial()
{
	textures.clear();
}

void ComponentMaterial::Bind()
{
	if(texture) texture->Bind();
}

void ComponentMaterial::Unbind()
{
	if (texture) texture->Unbind();
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
		if (currentTexture && currentTexture == texture.get())
			ImGui::TextColored({ 255,255,0,255 }, "Path: %s", texture->GetPath().c_str());
		else if (currentTexture == checkersTexture.get())
			ImGui::TextColored({ 255,255,0,255 }, "Default checkers texture");
		else
			ImGui::TextColored({ 255,255,0,255 }, "Path: This mesh does not have a texture");

		static bool checkers = currentTexture;
		if (currentTexture == checkersTexture.get()) checkers = true;
		else checkers = false;

		if (ImGui::Checkbox("Set checkers", &checkers))
		{
			if (checkers) currentTexture = checkersTexture.get();
			else if (texture) currentTexture = texture.get();
			else currentTexture = nullptr;
		}

		

		if (ImGui::Button("Change Texture"))
		{
			menuSelectTex = !menuSelectTex;

			if (menuSelectTex)
			{
				textures = app->editor->assetsPanel->textures;
			}
		}

		if (menuSelectTex)
			ShowTexturesMenu();


		ImGui::NewLine();
		ImGui::BulletText("Current Texture: ");
		if (texture)
			ImGui::Image((void*)texture->GetID(), { 150,150 });
		else
		{
			ImGui::SameLine();
			ImGui::Text("No texture.");
		}
		if (currentTexture == texture.get())
		{
			ImGui::BulletText("References to texture %i", texture.use_count() - 1); // -1 because of the resources map
		}
	}
}

void ComponentMaterial::ShowTexturesMenu()
{
	ImGui::Begin("Select Texture", &menuSelectTex);
	
	ImGui::Columns(2, 0, false);

	for (std::vector<Texture*>::const_iterator it = textures.begin(); it != textures.end(); it++)
	{
		std::string name = (*it)->GetName();

		std::string nameShow = name.substr(0, name.find_last_of("_"));
		nameShow = nameShow.substr(0, nameShow.find_last_of("_"));

		ImGui::ImageButton((ImTextureID)(*it)->GetID(), { 100,100 });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{ // TODO: need to change with resource manager
			std::shared_ptr<Resource> tex = ResourceManager::GetInstance()->IsAlreadyLoaded((*it)->GetPath().c_str());
			AddTexture(std::static_pointer_cast<Texture>(tex), 0);
		}

		ImGui::Text(nameShow.c_str());

		ImGui::NextColumn();
	}

	if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left || ImGuiMouseButton_Right))
		menuSelectTex = false;

	ImGui::End();
}

void ComponentMaterial::AddTexture(std::shared_ptr<Texture> tex, int modelUuid)
{
	texture = tex;
	currentTexture = texture.get();
	if (texture)
	{
		std::string initialPath = texture->GetLibraryPath();
		int s = initialPath.find("__");
		std::string p = initialPath.substr(0, s);

		// Need to recreate meta file?

		p += "__" + std::to_string(modelUuid) + ".kbtexture";
		texture->SetLibraryPath(p);
	
		texture->CreateMetaDataFile(texture->GetAssetsPath().c_str());
	}
}

JSON_Value* ComponentMaterial::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	json_object_set_number(obj, "Type", 2);

	if (currentTexture == checkersTexture.get())
	{
		json_object_set_string(obj, "path", "Checkers");
		json_object_set_number(obj, "uuid", -1);
	}
	if (texture && currentTexture == texture.get())
	{
		json_object_set_number(obj, "uuid", texture->uuid);
		json_object_set_string(obj, "path", texture->GetLibraryPath().c_str());
	}

	return value;
}

void ComponentMaterial::Load(JSON_Object* obj, GameObject* parent)
{
	//delete texture;
	currentTexture = nullptr;

	std::string path = json_object_dotget_string(obj, "path");
	if (path == "Checkers") currentTexture = checkersTexture.get();
	else
	{
		int uuid = json_object_get_number(obj, "uuid");
		//texName += "__" + std::to_string(uuid);
		//texture = TextureLoader::GetInstance()->LoadTextureCustomFormat(path);
		//ResourceManager::GetInstance()->CreateNewResource()
		if (ResourceManager::GetInstance()->IsAlreadyLoaded(uuid))
		{
			texture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->GetResource(uuid));
			//texture = tex.get();
			currentTexture = texture.get();
		}
		
		/*texture->uuid = uuid;
		currentTexture = texture;*/
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
	//checkersTexture = new Texture(checkerImage, CHECKERS_WIDTH, CHECKERS_HEIGHT, "Checkers");
	//currentTexture = checkersTexture;
	//return checkersTexture;
}