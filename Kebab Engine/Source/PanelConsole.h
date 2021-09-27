#pragma once

#include "Panel.h"

class ConsolePanel : public Panel
{
public:
	ConsolePanel();
	virtual ~ConsolePanel();

	bool Update(float dt) override;
	void Draw() override;

    ImGuiTextBuffer     Buf;
    bool                ScrollToBottom;

	void ClearConsole();

	void AddLog(const char* fmt);
};
