#pragma once

#include "Panel.h"

class ConsolePanel : public Panel
{
public:
	ConsolePanel();
	virtual ~ConsolePanel();

	void OnRender(float dt) override;

	void ClearConsole();

	void AddLog(const char* fmt, ...);

private:
	ImGuiTextBuffer buf;

	bool scrollMax;
};
