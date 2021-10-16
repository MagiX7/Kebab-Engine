#pragma once

#include "Buffer.h"

#include "GL/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

enum class TextureType
{
	NONE = 0,
	DIFFUSE,
	SPECULAR,
	HEIGHT_MAP,
};

class Texture
{
public:
	//Texture(unsigned char* pixels, int width, int height);
	Texture(const char* fileName);
	virtual ~Texture();

	void Bind(unsigned int index = 0) const;
	void Unbind() const;

private:
	unsigned int rendererID;

	int width;
	int height;
	int depth;
	int bpp;

	ILubyte* data;

};