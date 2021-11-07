#include "Application.h"
#include "FileSystem.h"

#include "TextureLoader.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

#define ASSETS_DIR "Assets/3D Models/"
#define CUSTOM_DIR "Library/Textures/"
#define CUSTOM_EXTENSION ".kbtexture"

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
		//textures.push_back(ret);

		SaveTextureCustomFormat(ret);
		//ret = LoadTextureCustomFormat(ret->GetName());

		ilDeleteImage(tmp);

		LOG_CONSOLE("\nLoaded image from %s", fileName);
	}

	ilBindImage(0);
	
	return ret;
}

Texture* TextureLoader::LoadTextureCustomFormat(std::string name)
{
	Texture* ret = nullptr;

	std::string n = CUSTOM_DIR + name + CUSTOM_EXTENSION;

	SDL_RWops* file = app->fileSystem->Load(n.c_str());

	unsigned int size = file->size(file);

	char* buffer = new char[size];
	app->fileSystem->Load(n.c_str(), &buffer);

	if (ilLoadL(IL_DDS, buffer, size))
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ret = new Texture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), name);
		textures.push_back(ret);

		//ret = new Texture(buffer, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), name);
		LOG_CONSOLE("Custom file format texture %s loaded!", name);
	}
	else LOG_CONSOLE("Could not load custom file format texture %s", name);

	delete[] buffer;
	
	return ret;
}

void TextureLoader::SaveTextureCustomFormat(Texture* tex)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	if (size > 0)
	{
		data = new ILubyte[size]; // Allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{
			std::string n = CUSTOM_DIR + tex->GetName() + CUSTOM_EXTENSION;
			tex->SetPath(n);
			app->fileSystem->Save(n.c_str(), data, size);
		}
		delete[] data;
	}
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