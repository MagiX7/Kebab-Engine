#include "Application.h"
#include "Input.h"
#include "Editor.h"
#include "FileSystem.h"

#include "PanelHierarchy.h"
#include "Shader.h"

#include "GameObject.h"
#include "ComponentMaterial.h"
#include "Material.h"
#include "Shader.h"

#include "PanelEditShader.h"

#include <fstream>

PanelEditShader::PanelEditShader()
{
	active = false;
}

PanelEditShader::~PanelEditShader()
{
}

void PanelEditShader::OnRender(float dt)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	const std::string title = "Editing " + shader->GetName();

	auto cpos = editor.GetCursorPosition();
	ImGui::Begin(title.c_str(), &active, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

	if (ImGui::IsItemFocused())
	{
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				std::string textToSave = editor.GetText();
				shader->UpdateSourceCode(textToSave);
			}
		}
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string textToSave = editor.GetText();
				shader->UpdateSourceCode(textToSave);

				const char* a = "Assets/Resources/Shaders/test.shader";
				app->fileSystem->Save(a, (void*)editor.GetText().c_str(), sizeof(char) * editor.GetText().size());

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
		
		/*ImGui::Separator();

		if (ImGui::Button("Set Current GameObject Shader"))
		{
			ComponentMaterial* matComp = (ComponentMaterial*)app->editor->hierarchyPanel->currentGO->GetComponent(ComponentType::MATERIAL);
			if (matComp)
			{
				SetFileToEdit(matComp->GetMaterial()->GetShader()->GetPath().c_str());
			}
			else LOG("Select a Game Object with shader on it");
		}*/
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

	editor.Render(title.c_str());
	ImGui::End();
}

void PanelEditShader::SetFileToEdit(const char* file)
{
	fileToEdit = file;
	std::ifstream t(fileToEdit);
	if (t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}
}

void PanelEditShader::SetShader(Shader* s)
{
	shader = s;
}
