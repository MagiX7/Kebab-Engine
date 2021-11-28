#pragma once

#include "Panel.h"

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

private:

	// Settings
	bool mipmap;
	bool anystropy;


	// Filters
	bool gaussianBlur;
	bool contrast;
	bool averageBlur;
	bool alienify;
	bool equalization;
	bool gammaCorrection;
	bool negativity;
	bool noise;
	bool pixelization;
	bool sharpening;

	float contrastAmount;
	float sharpenAmount;
	float sharpenIterations;
};