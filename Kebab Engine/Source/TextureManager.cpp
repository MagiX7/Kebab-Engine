#include "TextureManager.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

TextureManager::TextureManager() : Module(true)
{
	name = "textureManager";

	ilInit();
	iluInit();

	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);
}

TextureManager::~TextureManager()
{
}

bool TextureManager::CleanUp()
{
	ilShutDown();

	return true;
}

Texture* TextureManager::CreateTexture(const char* path)
{
	Texture* tex = new Texture(path);
	
	if (tex) textures.push_back(tex);

	return tex;
}