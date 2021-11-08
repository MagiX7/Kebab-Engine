#pragma once

#include "Panel.h"

#include <vector>
#include <string>

class GameObject;

enum class AssetType
{
	MODEL,
	TEXTURE,
};

struct Asset
{
	std::string name;
	std::string path;

	AssetType type;

	GameObject* gameObj;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void OnRender(float dt) override;

	void AddAsset(GameObject* gameObj);

	void DisplayAsset(Asset* asset);
	void DisplayPopMenu(Asset* asset);

private:

	std::vector<Asset*> assets;

	bool popUpMenu;
};