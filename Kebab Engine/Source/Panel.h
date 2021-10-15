#pragma once

#include "imgui/imgui.h"

class Panel
{
public:
	Panel() {}
	virtual ~Panel() {}

	virtual void OnRender(float dt) {}

	bool active;

protected:
	float scroll;
};