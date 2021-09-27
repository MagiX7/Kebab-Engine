#pragma once

#include "Module.h"

#define MAX_IT_HIST 100

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

	float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;

};