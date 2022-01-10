#pragma once

#include "Panel.h"

#include "TextEditor.h"

class PanelEditShader : public Panel
{
public:
	PanelEditShader();
	virtual ~PanelEditShader();

	void OnRender(float dt) override;

	void SetFileToEdit(const char* file);

private:
	TextEditor editor;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::GLSL();
	
	const char* fileToEdit;

};