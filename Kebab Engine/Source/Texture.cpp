#include "Application.h"
#include "Texture.h"

#include "Globals.h"

#define CHECKERS_HEIGHT 20
#define CHECKERS_WIDTH 20

Texture::Texture(const char* fileName)
{
	char* f = (char*)fileName;
	rendererID = ilutGLLoadImage(f);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	bpp = ilGetInteger(IL_IMAGE_BPP);
	depth = ilGetInteger(IL_IMAGE_DEPTH);
	data = ilGetData();

	//iluFlipImage();

	glBindTexture(GL_TEXTURE_2D, rendererID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//rendererID = ilutGLBindTexImage();
	//glGenTextures(1, &rendererID);
	//glBindTexture(GL_TEXTURE_2D, rendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_ARRAY_BUFFER, 0);
	ilBindImage(0);


	//LOG_CONSOLE("Loaded image with name %s", fileName);
}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int index) const
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}