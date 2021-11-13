#pragma once

#include "Panel.h"

#include "Math/float2.h"
#include "Math/float4.h"

class ComponentCamera;
class FrameBuffer;

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void OnRender(FrameBuffer* frameBuffer);

	inline const float4& GetDimensions() const { return dimensions; }

private:

	float2 size;
	float4 dimensions;
};