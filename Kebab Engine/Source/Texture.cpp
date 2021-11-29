#include "Application.h"
#include "Texture.h"

#include "Globals.h"

#define CHECKERS_HEIGHT 50
#define CHECKERS_WIDTH 50


Texture::Texture(void* data, int width, int height, std::string path/*, const TextureProperties& props*/)
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

	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind(unsigned int index) const
{
	//glActiveTexture(GL_TEXTURE0 + index);
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

	json_object_set_number(obj, "model uuid", uuid);

	JSON_Value* arrVal = json_value_init_array();
	JSON_Array* arr = json_value_get_array(arrVal);



}
