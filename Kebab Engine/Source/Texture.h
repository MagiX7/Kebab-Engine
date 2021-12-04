#pragma once

#include "Resource.h"

#include "Buffer.h"

#include "TextureProperties.h"

#include "GL/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"


//namespace TextureSettings
//{
//	enum Compression
//	{
//		DXTC_FORMAT = 0,
//		DXT1,
//		DXT2,
//		DXT3,
//		DXT4,
//		DXT5,
//		DXT_NO_COMP
//	};
//}

enum class TextureType
{
	NONE = 0,
	DIFFUSE,
	SPECULAR,
	HEIGHT_MAP,
};

class Texture : public Resource
{
public:
	//Texture(unsigned char* pixels, int width, int height);
	Texture();
	Texture(void* data, int width, int height, std::string path/*, const TextureProperties& props*/);
	//Texture();
	virtual ~Texture();

	void SetData(void* data, int width, int height);
	void UpdateData(void* data);
	//void UpdateData(const TextureProperties& props);
	void Clear();
	void Reimport(const TextureProperties& props);

	void Bind(unsigned int index = 0) const;
	void Unbind() const;

	inline const std::string& GetPath() const { return path; }
	inline void SetPath(const std::string& p) { path = p; }
	inline const unsigned int& GetID() const { return rendererID; }

	inline const std::string& GetName() const { return name; }

	inline void SetType(TextureType type) { this->type = type; }
	inline const TextureType& GetType() const { return type; }

	inline void SetProperties(const TextureProperties& p) { props = p; }
	
	void CreateMetaDataFile(const char* assetsFile);


private:
	unsigned int rendererID;
	TextureType type;

	int width;
	int height;
	int depth;
	int bpp;

	ILubyte* data;

	std::string path;
	std::string name;


	TextureProperties props;
};