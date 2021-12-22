#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "Model.h"

#include "ResourceManager.h"
#include "Editor.h"

#include "PanelAssets.h"

#include "ComponentTransform.h"
#include "Material.h"
#include "Shader.h"

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

	material = new Material();
}

ComponentMaterial::~ComponentMaterial()
{
	texture.reset();
	checkersTexture.reset();
	currentTexture = nullptr;

	/*for (auto& tex : textures)
		delete tex;*/

	textures.clear();
}

void ComponentMaterial::Bind()
{
	if(texture && currentTexture == texture.get())
		texture->Bind();
	ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
	float4x4 mat = trans->GetGlobalMatrix();	
	material->Bind(mat);
}

void ComponentMaterial::Unbind()
{
	if (texture && currentTexture == texture.get()) texture->Unbind();
	if (material) material->Unbind();
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
	if (ImGui::CollapsingHeader("Material"))
	{
		if (currentTexture && currentTexture == texture.get())
			ImGui::TextColored({ 255,255,0,255 }, "Path: %s", texture->GetLibraryPath().c_str());
		else
			ImGui::TextColored({ 255,255,0,255 }, "Path: This mesh does not have a texture");

		if (ImGui::Checkbox("Unselect texture", &unselect))
		{
			if (unselect)
				currentTexture = nullptr;
			else
				currentTexture = texture.get();
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

		ImGui::Separator();
		ImGui::Separator();

		ImGui::Text("Material: %s", material->GetName().c_str());
		ImGui::BulletText("Shader: %s", material->GetShader()->GetName().c_str());
		
		static float3 col = material->ambientColor;
		if (ImGui::ColorEdit3("Material Color", col.ptr()))
		{
			material->ambientColor = col;
		}

		if (ImGui::Button("Change Shader"))
		{
			ChangeShaderWindow();
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
		{
			std::shared_ptr<Resource> tex = ResourceManager::GetInstance()->IsAlreadyLoaded((*it)->GetLibraryPath());
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
	currentTexture = nullptr;

	std::string path = json_object_dotget_string(obj, "path");
	if (path == "Checkers") currentTexture = checkersTexture.get();
	else
	{
		int uuid = json_object_get_number(obj, "uuid");

		if (std::shared_ptr<Texture> tex = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->IsAlreadyLoaded(path)))
		{
			texture = tex;
			currentTexture = texture.get();
		}
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
}

void ComponentMaterial::ChangeShaderWindow()
{
	ImGui::Begin("Select New Shader");

	// TODO: Display all the shaders

	ImGui::End();
}
