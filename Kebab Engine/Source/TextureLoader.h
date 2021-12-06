#pragma once

#include "Texture.h"

#include <vector>

class TextureLoader
{
public:
	static TextureLoader* GetInstance();

	virtual ~TextureLoader();

	Texture* LoadTexture(const char* fileName, const TextureProperties& props = TextureProperties());
	Texture* LoadTextureCustomFormat(const std::string& path, const TextureProperties& props = TextureProperties());

	void SaveTextureCustomFormat(Texture* tex, int uuid);

	void CleanUp();

private:
	static TextureLoader* instance;

	TextureLoader();
	TextureLoader(const TextureLoader&) {}
	void operator=(const TextureLoader&) {};

	std::vector<Texture*> textures;
};