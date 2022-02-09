#pragma once
#ifndef SR_RASTERIZER_H
#define SR_RASTERIZER_H
#include "REnderer/Vertex.h"
#include "Utils/Window.h"

class Rasterizer
{
public:
	Rasterizer(Window* window);
	~Rasterizer() = default;

	void ScreenMapping(V2F& v);

	void RasterizeTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void RasterizeLine(const V2F& v1, const V2F& v2);
	void RasterizePoint(const V2F& v);

private:
	void ScanUpTriangle(V2F& v1, V2F& v2, V2F& v3);
	void ScanDownTriangle(V2F& v1, V2F& v2, V2F& v3);
	void ScanLine(const V2F& v1, const V2F& v2);

private:
	Window* m_window;
};

#endif