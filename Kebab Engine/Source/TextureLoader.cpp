#include "Application.h"

#include "TextureLoader.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

#define ASSETS_DIR "Assets/3D Models/"

TextureLoader* TextureLoader::instance = nullptr;

TextureLoader* TextureLoader::GetInstance()
{
	if (!instance) instance = new TextureLoader();
	return instance;
}

TextureLoader::TextureLoader()
{
	ilInit();
	iluInit();
	ilutInit();

	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);
}

TextureLoader::~TextureLoader()
{
	/*for (int i = 0; i < textures.size(); ++i)
	{
		delete(textures[i]);
		textures[i] = nullptr;
	}

	delete(instance);
	instance = nullptr;*/
}

Texture* TextureLoader::LoadTexture(const char* fileName)
{
	Texture* ret = nullptr;

	ILuint tmp;
	tmp = ilGenImage();
	ilBindImage(tmp);

	/*std::string dir = ASSETS_DIR;
	dir.append(fileName);*/

	if (!ilLoadImage(fileName))
	{
		//LOG_CONSOLE("Could not load Image from %s", fileName);
	}
	else
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ret = new Texture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), fileName);
		textures.push_back(ret);

		app->fileSystem->SaveTextureCustomFormat(ret);

		ilDeleteImage(tmp);

		LOG_CONSOLE("\nLoaded image from %s", fileName);
	}

	ilBindImage(0);
	
	return ret;
}

void TextureLoader::CleanUp()
{
	/*for (int i = 0; i < textures.size(); ++i)
	{
		delete(textures[i]);
		textures[i] = nullptr;
	}*/

	ilShutDown();

	delete(instance);
	instance = nullptr;
}