#include "Application.h"
#include "FileSystem.h"

#include "TextureLoader.h"
#include "Texture.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT 50
#define CHECKERS_WIDTH 50


Texture::Texture(void* data, int width, int height, std::string path)
	: Resource(ResourceType::TEXTURE)
{
	this->width = width;
	this->height = height;
	this->path = path;
	this->type = TextureType::DIFFUSE;

	int start = path.find_last_of('\\');
	if (start <= 0)
		start = path.find_last_of('/');

	int end = path.find_last_of('.');

	name = path.substr(start + 1, end - start - 1);

	SetData(data, width, height);
}

Texture::Texture() : Resource(ResourceType::TEXTURE)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &rendererID);
}

void Texture::SetData(void* data, int width, int height)
{
	if (rendererID)
	{
		glDeleteTextures(1, &rendererID);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &rendererID);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_NEAREST*/ GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST*/ GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::UpdateData(void* data)
{
	glBindTexture(GL_TEXTURE_2D, rendererID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Clear()
{
	if (rendererID)
		glDeleteTextures(1, &rendererID);

	width = 0;
	height = 0;
}

void Texture::ReLoad(const TextureProperties& props, bool save, bool generateLibFile)
{
	if (rendererID)
		glDeleteTextures(1, &rendererID);

	width = 0;
	height = 0;

	ILuint tmp;
	tmp = ilGenImage();
	ilBindImage(tmp);

	if (ilLoadImage(path.c_str()))
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (props.mipmap) iluBuildMipmaps();
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

		this->props = props;

		SetData(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

		ilBindImage(0);
	}

	if (save)
	{
		std::string p = assetsFile + ".meta";
		app->fileSystem->Remove(p.c_str());
		CreateMetaDataFile(assetsFile.c_str());
	}
	if (generateLibFile)
	{
		TextureLoader::GetInstance()->SaveTextureCustomFormat(this, 0);
	}

	ilDeleteImage(tmp);
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

void Texture::CreateMetaDataFile(const char* assetsFile)
{
	hasMetaFile = true;

	std::string p = assetsFile;
	p += ".meta";
	metaFile = p;

	JSON_Value* value = json_value_init_object();
	JSON_Object* obj = json_value_get_object(value);

	json_object_set_number(obj, "texture uuid", uuid);
	json_object_set_string(obj, "texture library path", libraryFile.c_str());
	json_object_set_string(obj, "texture assets path", this->assetsFile.c_str());


	// TODO: Save compression properly
	json_object_dotset_number(obj, "properties.compression", props.compression);
	json_object_dotset_boolean(obj, "properties.mipmap", props.mipmap);
	json_object_dotset_boolean(obj, "properties.gaussianBlur", props.gaussianBlur);
	if(props.gaussianBlur)
		json_object_dotset_number(obj, "properties.gaussianBlurIterations", props.gaussianBlur);

	json_object_dotset_boolean(obj, "properties.averageBlur", props.averageBlur);
	if(props.averageBlur)
		json_object_dotset_number(obj, "properties.averageBlurIterations", props.averageBlurIterations);
	
	json_object_dotset_boolean(obj, "properties.contrast", props.contrast);
	if (props.contrast)
		json_object_dotset_number(obj, "properties.contrastAmount", props.contrastAmount);

	json_object_dotset_boolean(obj, "properties.alienify", props.alienify);
	json_object_dotset_boolean(obj, "properties.equalization", props.equalization);
	json_object_dotset_boolean(obj, "properties.gammaCorrection", props.gammaCorrection);
	if(props.gammaCorrection)
		json_object_dotset_number(obj, "properties.gammaCorrectionAmount", props.gammaCorrectionAmount);

	json_object_dotset_boolean(obj, "properties.negativity", props.negativity);
	json_object_dotset_boolean(obj, "properties.noise", props.noise);
	if (props.noise)
		json_object_dotset_number(obj, "properties.noiseAmount", props.noiseAmount);

	json_object_dotset_boolean(obj, "properties.pixelizations", props.pixelization);
	if(props.pixelization)
		json_object_dotset_number(obj, "properties.pixelsSize", props.pixelsSize);

	json_object_dotset_boolean(obj, "properties.sharpening", props.sharpening);
	if (props.sharpening)
	{
		json_object_dotset_number(obj, "properties.sharpeningAmount", props.sharpeningAmount);
		json_object_dotset_number(obj, "properties.sharpeningIterations", props.sharpeningIterations);
	}

	json_serialize_to_file_pretty(value, p.c_str());
	json_value_free(value);
}