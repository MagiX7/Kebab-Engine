#pragma once

#include "Panel.h"

class GameDebugInfoPanel : public Panel
{
public:
	GameDebugInfoPanel();
	virtual ~GameDebugInfoPanel();

	void OnRender(float dt) override;

private:


};