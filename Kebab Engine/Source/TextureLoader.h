#pragma once

#include "Texture.h"

class TextureLoader
{
public:
	static TextureLoader* GetInstance();

	virtual ~TextureLoader();

	Texture* LoadTexture(const char* fileName);

private:
	static TextureLoader* instance;

	TextureLoader() {};
	TextureLoader(const TextureLoader&) {}
	void operator=(const TextureLoader&) {};

	std::vector<Texture*> textures;
};