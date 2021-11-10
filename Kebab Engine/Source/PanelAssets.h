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

	void AddAsset(GameObject* gameObj);

	void DisplayAssets();
	void DisplayPopMenu();

private:

	std::vector<Asset*> assets;

	std::string currentFolder;
	std::string entryFolder;

	Texture* folderTex;
	Texture* modelTex;

	int i = 0;

	std::string popUpItem;
};