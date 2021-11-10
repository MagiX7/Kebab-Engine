#pragma once

#include "Panel.h"

#include <vector>
#include <string>

class GameObject;
class Texture;

enum class AssetType
{
	FBX,
	PNG,
	NONE,
};

struct Asset
{
	std::string name;
	std::string path;

	AssetType type = AssetType::NONE;

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
	void DisplayPopMenu(Asset* asset);

private:

	std::vector<Asset*> assets;

	std::string currentFolder;
	std::string entryFolder;

	Texture* folderTex;
	Texture* modelTex;

	int i = 0;

	bool popUpMenu;
};