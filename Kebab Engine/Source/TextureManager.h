#pragma once

#include "Module.h"

#include <vector>

class Texture;

class TextureManager : public Module
{
public:
	TextureManager();
	virtual ~TextureManager();

	bool CleanUp() override;

	Texture* CreateTexture(const char* path);

private:
	std::vector<Texture*> textures;

};