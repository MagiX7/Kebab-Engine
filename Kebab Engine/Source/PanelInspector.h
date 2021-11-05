#pragma once

#include "Panel.h"

class InspectorPanel : public Panel
{
public:
	InspectorPanel();
	virtual ~InspectorPanel();

	void OnRender(float dt) override;

private:


};