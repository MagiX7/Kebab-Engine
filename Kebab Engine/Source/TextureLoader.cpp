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

Texture* TextureLoader::LoadTexture(const char* fileName, const TextureProperties& props)
{
	Texture* ret = nullptr;

	ILuint tmp;
	tmp = ilGenImage();
	ilBindImage(tmp);

	/*std::string dir = ASSETS_DIR;
	dir.append(fileName);*/

	if (!ilLoadImage(fileName))
	{
		LOG_CONSOLE("Could not load Image from %s", fileName);
	}
	else
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (props.mipmap) iluBuildMipmaps();
		//if (props.anystropy) 
		if (props.alienify) iluAlienify();
		if (props.gaussianBlur) iluBlurGaussian(props.gaussianBlurIterations);
		if (props.averageBlur) iluBlurAvg(props.averageBlurIterations);
		if (props.contrast) iluContrast(props.contrastAmount);
		if (props.equalization) iluEqualize();
		if (props.gammaCorrection) iluGammaCorrect(props.gammaCorrectionAmount);
		if (props.negativity) iluNegative();
		if (props.noise) iluNoisify(props.noiseAmount);
		if (props.pixelization) iluPixelize(props.pixelsSize);
		if (props.sharpening) iluSharpen(props.sharpeningAmount, props.sharpeningIterations);

		ret = new Texture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), fileName);
		ret->SetProperties(props);

		//SaveTextureCustomFormat(ret); <-- Done in ResourceManager
		//ret = LoadTextureCustomFormat(ret->GetName());

		ilDeleteImage(tmp);

		LOG_CONSOLE("\nLoaded image from %s", fileName);
	}

	ilBindImage(0);
	
	return ret;
}

Texture* TextureLoader::LoadTextureCustomFormat(const std::string& path)
{
	Texture* ret = nullptr;

	char* buffer;
	int size = app->fileSystem->Load(path.c_str(), &buffer);

	if (ilLoadL(IL_DDS, buffer, size))
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ret = new Texture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), path);
		textures.push_back(ret);

		//ret = new Texture(buffer, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), name);
		LOG_CONSOLE("Custom file format texture %s loaded!", path.c_str());
		
		delete[] buffer;
	}
	else LOG_CONSOLE("Could not load custom file format texture %s", path.c_str());

	
	return ret;
}

void TextureLoader::SaveTextureCustomFormat(Texture* tex, int uuid)
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
			std::string n = CUSTOM_DIR + tex->GetName() + "__" + std::to_string(uuid) + CUSTOM_EXTENSION;
			tex->SetLibraryPath(n);
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