#pragma once

#include "Panel.h"
#include "Buffer.h"

#include "Math/float2.h"

class ViewportPanel : public Panel
{
public:
	ViewportPanel();
	virtual ~ViewportPanel();

	void OnRender(FrameBuffer* frameBuffer);

	inline float2 const& GetViewportSize() const { return viewportSize; }

private:
	float2 viewportSize = { 0,0 };
};