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

	void OnRender(FrameBuffer* frameBuffer, const ImGuizmo::OPERATION& op, const ImGuizmo::MODE& mode);

	inline float2 const& GetViewportSize() const { return viewportSize; }

	float4 GetDimensions();
	bool IsHovered();

private:
	void DrawGuizmo(const ImGuizmo::OPERATION& op, const ImGuizmo::MODE& mode);

private:
	float2 viewportSize;

	float4 viewportDimensions;

	bool hovered;
};

class ScenePreviewPanel : public Panel
{
public:
	ScenePreviewPanel();
	virtual ~ScenePreviewPanel();

	void OnRender(FrameBuffer* frameBuffer);

	void SetSize(const float2& newSize);

private:

	float2 size;
	float4 dimensions;


	//friend float4 ViewportPanel::GetDimensions();
};