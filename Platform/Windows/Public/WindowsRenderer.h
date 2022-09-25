#pragma once
#include "Render/Public/Renderer.h"

class Rasterizer;

class WindowsRenderer : public Renderer
{
public:
	WindowsRenderer(Primitive type);
	virtual ~WindowsRenderer();

private:
	Rasterizer* m_rasterizer;
};