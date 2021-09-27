#pragma once

#include "imgui/imgui.h"

class Panel
{
public:
	Panel() {}
	virtual ~Panel() {}

	//bool Start();
	virtual bool Update(float dt) { return true; }
	virtual void Draw() {}
	//bool CleanUp();

private:

};