#pragma once

#include "Panel.h"
#include "ModelProperties.h"

#include <string>

class PanelImportModel : public Panel
{
public:
	PanelImportModel();
	virtual ~PanelImportModel();

	void OnRender(float dt) override;

	void HelpMarker(const char* text);

	inline void SetAssetsPath(const std::string& path) { assetsPath = path; }
	inline const std::string& GetAssetsPath() { return assetsPath; }

private:
	ModelProperties props;

	std::string assetsPath;

};