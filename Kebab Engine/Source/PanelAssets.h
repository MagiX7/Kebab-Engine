#pragma once

#include "Panel.h"

#include <vector>
#include <string>

class GameObject;
class Texture;

struct Asset
{
	std::string path;

	GameObject* gameObj;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void OnRender(float dt) override;

	void LoadAssetsToCustom();

	void DisplayAssets();
	void DisplayPopMenu();

private:

	std::vector<Asset*> assets;

	std::string currentFolder;
	std::string entryFolder;

	Texture* folderTex;
	Texture* modelTex;
	Texture* pngTex;

	std::string popUpItem;

	float columns;
};