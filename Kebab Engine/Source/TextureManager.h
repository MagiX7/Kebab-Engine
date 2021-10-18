#pragma once

#include "Module.h"
#include "Texture.h"

#include <vector>
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