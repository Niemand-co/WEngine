#pragma once
#ifndef SR_RASTERIZER_H
#define SR_RASTERIZER_H
#include "REnder/Vertex.h"
#include "Utils/Window.h"
#include "Utils/Shader.h"

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer() = default;

	void ScreenMapping(V2F& v);

	void RasterizeTriangle(const V2F& v1, const V2F& v2, const V2F& v3);
	void RasterizeLine(const V2F& v1, const V2F& v2);
	void RasterizePoint(const V2F& v);

	void SetPixelShader(Shader* shader);
private:
	void ScanUpTriangle(V2F& v1, V2F& v2, V2F& v3);
	void ScanDownTriangle(V2F& v1, V2F& v2, V2F& v3);
	void ScanLine(const V2F& v1, const V2F& v2);

private:
	Shader* m_shader;
};

#endif