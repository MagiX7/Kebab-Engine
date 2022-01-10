#include "Application.h"
#include "Renderer3D.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "Model.h"

#include "ResourceManager.h"
#include "Editor.h"

#include "PanelAssets.h"
#include "PanelEditShader.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Material.h"
#include "Shader.h"


#include "imgui/imgui.h"
#include "TextEditor.h"

#include <fstream>

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT 40
#define CHECKERS_WIDTH 40

#define MAX_TIME_TO_REFRESH_SHADER 10 // Seconds

ComponentMaterial::ComponentMaterial(GameObject* compOwner)
{
	this->parent = compOwner;
	this->type = ComponentType::MATERIAL;
	this->active = true;

	texture = nullptr;
	currentTexture = nullptr;
	checkersTexture = nullptr;

	menuSelectTex = false;
	closeShaderWindow = false;

	SetCheckersTexture();

	material = new Material(*app->renderer3D->GetDefaultMaterial());

	updateShaderTimer = 0.0f;

	panelEditShader = new PanelEditShader();	
}

ComponentMaterial::~ComponentMaterial()
{
	texture.reset();
	checkersTexture.reset();
	currentTexture = nullptr;
	
	delete material;
}

void ComponentMaterial::Update(float dt)
{
	if (updateShaderTimer <= MAX_TIME_TO_REFRESH_SHADER)
	{
		updateShaderTimer += dt;
	}
	else
	{
		updateShaderTimer = 0.0f;
		material->GetShader()->Refresh();
	}
}

void ComponentMaterial::Bind(ComponentCamera* cam)
{
	if(texture && currentTexture == texture.get())
		texture->Bind();
	ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
	float4x4 mat = trans->GetGlobalMatrix();	
	material->Bind(mat, cam);
}

void ComponentMaterial::Unbind()
{
	if (texture && currentTexture == texture.get())
		texture->Unbind();
	material->Unbind();	
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
			menuSelectTex = !menuSelectTex;

		if (menuSelectTex)
			ShowTexturesMenu();


		ImGui::NewLine();
		ImGui::BulletText("Current Texture: ");
		if (texture)
		{
			ImGui::Image((void*)texture->GetID(), { 150,150 });
			ImGui::DragFloat("Alpha", &material->textureAlpha, 0.01f, 0.0, 1.0);
		}
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

		ImGui::Spacing();

		ImGui::Text("Material: %s", material->GetName().c_str());
		ImGui::BulletText("Shader: %s", material->GetShader()->GetName().c_str());
		
		ImGui::SameLine();
		//static bool showEditShaderWindow = false;
		if (ImGui::Button("Open in editor"))
		{
			panelEditShader->active = true;
			panelEditShader->SetFileToEdit(material->GetShader()->GetPath().c_str());
		}

		if (ImGui::Button("Change Shader"))
			closeShaderWindow = !closeShaderWindow;
		
		if (closeShaderWindow)
			ChangeShaderWindow();

		ImGui::Spacing();
		ImGui::Spacing();
		
		//static float3 col = material->ambientColor;
		ImGui::ColorEdit3("Material Color", material->ambientColor.ptr());


		//std::unordered_map<std::string, float> uniforms = material->GetShader()->GetUniforms();
		std::list<UniformData> uniforms = material->GetShader()->GetUniforms();

		std::list<UniformData>::iterator it = uniforms.begin();
		for (; it != uniforms.end(); it++)
		{	
			ImGui::DragFloat(it->name.c_str(), &it->data);
		}

		if (panelEditShader->active)
			panelEditShader->OnRender(0);

		/*if (material->GetShader()->GetName() == "wave.shader")
		if (material->GetName() == "Wave")
		{
			ImGui::DragFloat("Frequency", &material->frequency, 0.1);
			ImGui::DragFloat("Speed", &material->speed, 0.01, -2, 2);
			ImGui::DragFloat("Amplitude", &material->amplitude, 0.01, -2, 2);
		}*/
		// 	ImGui::DragFloat("Noise Amount", &material->noiseAmount, 0.05);
		// 	ImGui::DragFloat("Foam Speed", &material->foamSpeed, 0.01, -2, 2);
		// 	ImGui::SameLine(0, -10.0f);
		// 	ImGui::TextDisabled("(?)");
		// 	if (ImGui::IsItemHovered())
		// 	{
		// 		ImGui::BeginTooltip();
		// 		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		// 		ImGui::TextUnformatted("Only works when there is a texture different than white");
		// 		ImGui::PopTextWrapPos();
		// 		ImGui::EndTooltip();
		// 	}
		// 	//ImGui::DragFloat2("Foam Direction", material->foamDir.ptr());
		// }

		ImGui::BulletText("Last time modified: %s", material->GetShader()->GetLastModifiedDate());

	}
}

void ComponentMaterial::ShowTexturesMenu()
{
	//ImGui::SetNextWindowPos(ImVec2(2, 2));

	ImGui::Begin("Select Texture", &menuSelectTex);
	
	std::vector<Texture*> textures = app->editor->assetsPanel->textures;

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

void ComponentMaterial::SetMaterial(Material* mat)
{
	material = mat;
}

Material* ComponentMaterial::GetMaterial()
{
	return material;
}

void ComponentMaterial::AddTexture(std::shared_ptr<Texture> tex, int modelUuid)
{
	/*if (tex && tex->GetResourceType() != ResourceType::TEXTURE) return;

	if (!tex)
	{
		texture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->IsAlreadyLoaded("Assets/Resources/white.png"));
	}*/

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

	if (material->GetShader())
	//if(material->GetName() == "Wave")
	{
		json_object_set_string(obj, "shader", material->GetShader()->GetPath().c_str());
		json_object_set_string(obj, "material_name", material->GetName().c_str());
		json_object_set_number(obj, "color_r", material->ambientColor.x);
		json_object_set_number(obj, "color_g", material->ambientColor.y);
		json_object_set_number(obj, "color_b", material->ambientColor.z);
		json_object_set_number(obj, "freq", material->frequency);
		json_object_set_number(obj, "speed", material->speed);
		json_object_set_number(obj, "amplitude", material->amplitude);
		json_object_set_number(obj, "alpha", material->textureAlpha);
		json_object_set_number(obj, "noiseAmount", material->noiseAmount);
		json_object_set_number(obj, "foamSpeed", material->foamSpeed);
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

		std::string shaderPath = "";

		if (json_object_get_string(obj, "shader") != nullptr)
			shaderPath = json_object_get_string(obj, "shader");

		material->SetName(json_object_get_string(obj, "material_name"));
		material->ambientColor.x = json_object_get_number(obj, "color_r");
		material->ambientColor.y = json_object_get_number(obj, "color_g");
		material->ambientColor.z = json_object_get_number(obj, "color_b");
		material->frequency = json_object_get_number(obj, "freq");
		material->speed = json_object_get_number(obj, "speed");
		material->amplitude = json_object_get_number(obj, "amplitude");
		material->textureAlpha = json_object_get_number(obj, "alpha");
		material->noiseAmount = json_object_get_number(obj, "noiseAmount");
		material->foamSpeed = json_object_get_number(obj, "foamSpeed");


		std::vector<Shader*> shaders = app->renderer3D->GetShaders();
		for (std::vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
		{
			if ((*it)->GetPath() == shaderPath)
			{
				material->SetShader((*it));
				break;
			}
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
	ImGui::Begin("Select the new Shader", &closeShaderWindow);

	ImGui::Columns(2, 0, false);

	std::vector<Shader*> shaders = app->renderer3D->GetShaders();

	for (std::vector<Shader*>::const_iterator it = shaders.begin(); it != shaders.end(); it++)
	{
		std::string name = (*it)->GetName();

		std::string nameShow = name.substr(0, name.find_last_of("_"));
		nameShow = nameShow.substr(0, nameShow.find_last_of("_"));
		nameShow = nameShow.substr(0, nameShow.find_last_of("."));

		if (ImGui::Button(name.c_str(), { 100,100 }))
		{
			material->SetShader((*it));
			if (nameShow == "wave")
				material->SetName("Wave");
			else
				material->SetName("Default");
			// Maybe need to change the path
		}

		ImGui::Text(nameShow.c_str());

		ImGui::NextColumn();
	}

	if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left || ImGuiMouseButton_Right))
		closeShaderWindow = false;

	ImGui::End();
}

void ComponentMaterial::ShowEditShaderWindow()
{
	//ImGui::Begin("Edit Shader");
	TextEditor editor;
	auto lang = TextEditor::LanguageDefinition::GLSL();

	//TextEditor::ErrorMarkers markers;
	//markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	//markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
	//editor.SetErrorMarkers(markers);

	static const char* fileToEdit = material->GetShader()->GetPath().c_str();

	std::ifstream t(fileToEdit);
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}

	//TextEditor::Breakpoints bpts;
	//bpts.insert(24);
	//bpts.insert(47);
	//editor.SetBreakpoints(bpts);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	auto cpos = editor.GetCursorPosition();

	static bool close = false;
	ImGui::Begin("Text Editor Demo", &close, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600)/*, ImGuiCond_FirstUseEver */ );

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string textToSave = editor.GetText();
				material->GetShader()->UpdateSourceCode(textToSave);

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

	editor.Render("TextEditor");
	ImGui::End();

}
