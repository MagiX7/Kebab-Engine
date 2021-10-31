#pragma once

#include "Panel.h"
#include "Buffer.h"

#include "Math/float2.h"
#include "Math/float4.h"

class ViewportPanel : public Panel
{
public:
	ViewportPanel();
	virtual ~ViewportPanel();

	void OnRender(FrameBuffer* frameBuffer);

	inline float2 const& GetViewportSize() const { return viewportSize; }

	float4 GetViewportDimensions();
	bool IsHovered();

private:
	float2 viewportSize;

	float4 viewportDimensions;

	bool hovered;
};