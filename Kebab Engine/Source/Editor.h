#pragma once

#include "Module.h"

class Editor : public Module
{
public:
	Editor(Application* app, bool startEnabled = true);
	virtual ~Editor();

	bool Start() override;
	bool Update(float dt) override;
	bool Draw(float dt) override;
	bool CleanUp() override;

private:
	bool showCase;

	float fpsLog[50];
	float msLog[50];
	int i;

};