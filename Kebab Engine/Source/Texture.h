#pragma once

#include "GL/glew.h"

class Texture
{
public:
	Texture(unsigned char* pixels, int width, int height);
	virtual ~Texture();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int rendererID;
};