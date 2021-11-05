#pragma once

#include "Texture.h"

class TextureLoader
{
public:
	static TextureLoader* GetInstance();

	virtual ~TextureLoader();

	Texture* LoadTexture(const char* fileName);
	Texture* LoadTextureCustomFormat(std::string name);

	void SaveTextureCustomFormat(const std::string& name);

	void CleanUp();

private:
	static TextureLoader* instance;

	TextureLoader();
	TextureLoader(const TextureLoader&) {}
	void operator=(const TextureLoader&) {};

	std::vector<Texture*> textures;
};