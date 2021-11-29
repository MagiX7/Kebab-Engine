#pragma once

#include <il.h>

struct TextureProperties
{
	ILint compression = IL_DXT5;

	bool mipmap = true;

	bool gaussianBlur = false;
	bool contrast = false;
	bool averageBlur = false;
	bool alienify = false;
	bool equalization = false;
	bool gammaCorrection = false;
	bool negativity = false;
	bool noise = false;
	bool pixelization = false;
	bool sharpening = false;

	int gaussianBlurIterations = 1;
	int averageBlurIterations = 1;
	float contrastAmount = 1.0f;
	float gammaCorrectionAmount = 1.0f;
	float noiseAmount = 1.0f;
	float pixelsSize = 1.0f;
	float sharpeningAmount = 1.0f;
	int sharpeningIterations = 1;

	// 20 items to save
};