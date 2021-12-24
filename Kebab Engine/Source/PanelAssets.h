#pragma once

#include "Panel.h"

#include <vector>
#include <string>

class GameObject;
class Texture;

class AssetsPanel : public Panel
{
public:
	AssetsPanel();
	virtual ~AssetsPanel();

	void OnRender(float dt) override;

	void LoadAssetsToCustom();

	void DisplayAssets();
	void DisplayPopMenu();
	void DisplayItemPopMenu();

	void RefreshAssets(float dt);

public:
	std::vector<Texture*> textures;

private:

	std::string currentFolder;
	std::string entryFolder;

	Texture* folderTex;
	Texture* modelTex;
	Texture* pngTex;

	std::string popUpItem;
	bool popUpMenu;

	float columns;
	bool rename;

	float timer;
};