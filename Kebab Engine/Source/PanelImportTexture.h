#pragma once

#include "Panel.h"
#include "TextureProperties.h"

#include <string>

class Texture;

namespace ImageCompression
{
	enum Type
	{
		DXTC_FORMAT = 0,
		DXT1,
		DXT2,
		DXT3,
		DXT4,
		DXT5,
		DXT_NO_COMP
	};
}

class ImportTexturePanel : public Panel
{
public:
	ImportTexturePanel();
	virtual ~ImportTexturePanel();

	void OnRender(float dt) override;

	void SetTexturePath(const char* assetsFile);

private:
	Texture* texture;
	std::string assetsPath;
	TextureProperties props;
};