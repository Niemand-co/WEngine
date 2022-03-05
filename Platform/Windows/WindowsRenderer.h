#pragma once
#include "Render/Renderer.h"

class Rasterizer;

class WindowsRenderer : public Renderer
{
public:
	WindowsRenderer(Primitive type);
	virtual ~WindowsRenderer();

	virtual void DrawByIndices(Entity* entity, Light* light, Camera* camera) override;
private:
	Rasterizer* m_rasterizer;
};