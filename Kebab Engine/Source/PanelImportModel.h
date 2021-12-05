#pragma once

#include "Panel.h"
#include "ModelProperties.h"

#include <string>
#include <memory>

class KbModel;

class PanelImportModel : public Panel
{
public:
	PanelImportModel();
	virtual ~PanelImportModel();

	void OnRender(float dt) override;

	void HelpMarker(const char* text);

	void SetAssetsPath(const std::string& path);
	inline const std::string& GetAssetsPath() { return assetsPath; }

private:
	ModelProperties props;
	std::shared_ptr<KbModel> model;

	std::string assetsPath;
	std::string extension;

};