#pragma once

#include "Panel.h"
#include "TextureProperties.h"

#include <string>
#include <memory>

class Texture;

namespace ImageCompression
{
	enum Type
	{
		DXT1 = 0,
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

	ILint GetCompressionFromType(int compression);

private:
	std::shared_ptr<Texture> texture;
	std::string assetsPath;
	TextureProperties props;
	TextureProperties initialProps = props;
};