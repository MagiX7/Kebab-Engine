#pragma once

#include "Panel.h"

class PanelLightSettings : public Panel
{
public:
	PanelLightSettings();
	virtual ~PanelLightSettings();

	void OnRender(float dt) override;

};