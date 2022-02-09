#include "Rasterizer.h"
#include "Math/Matrix.h"

Rasterizer::Rasterizer(Window* window)
	: m_window(window)
{
}

void Rasterizer::ScreenMapping(V2F& v)
{
	Matrix4x4f viewportMatrix = ViewportMatrix(0.0f, 0.0f, float(m_window->GetWidth()), float(m_window->GetHeight()));
	v.ScreenPos = v.ScreenPos * float(1.0 / v.ScreenPos.w);
	v.ScreenPos.w = 1.0f;
	v.ScreenPos = viewportMatrix * v.ScreenPos;
}

void Rasterizer::RasterizeTriangle(const V2F& v1, const V2F& v2, const V2F& v3)
{
	
	V2F v2fArray[3] = {v1, v2, v3};
	ScreenMapping(v2fArray[0]);
	ScreenMapping(v2fArray[1]);
	ScreenMapping(v2fArray[2]);
	if (v2fArray[0].ScreenPos.y > v2fArray[1].ScreenPos.y)
	{
		V2F tmp = v2fArray[0];
		v2fArray[0] = v2;
		v2fArray[1] = tmp;
	}
	if (v2fArray[1].ScreenPos.y > v2fArray[2].ScreenPos.y)
	{
		V2F tmp = v2fArray[1];
		v2fArray[1] = v2;
		v2fArray[2] = tmp;
	}
	if (v2fArray[0].ScreenPos.y > v2fArray[1].ScreenPos.y)
	{
		V2F tmp = v2fArray[0];
		v2fArray[0] = v2;
		v2fArray[1] = tmp;
	}

	if (v2fArray[0].ScreenPos.y == v2fArray[1].ScreenPos.y)
	{
		ScanUpTriangle(v2fArray[0], v2fArray[1], v2fArray[2]);
	}
	else if (v2fArray[1].ScreenPos.y == v2fArray[2].ScreenPos.y)
	{
		ScanDownTriangle(v2fArray[0], v2fArray[1], v2fArray[2]);
	}
	else
	{
		float weight = (v2fArray[2].ScreenPos.y - v2fArray[1].ScreenPos.y) / (v2fArray[2].ScreenPos.y - v2fArray[0].ScreenPos.y);
		V2F mid = V2F::Lerp(v2fArray[2], v2fArray[0], weight);
		ScanUpTriangle(mid, v2fArray[1], v2fArray[2]);
		ScanDownTriangle(v2fArray[0], mid, v2fArray[1]);
	}
}

void Rasterizer::RasterizeLine(const V2F& v1, const V2F& v2)
{
}

void Rasterizer::RasterizePoint(const V2F& v)
{
}

void Rasterizer::ScanUpTriangle(V2F& v1, V2F& v2, V2F& v3)
{
	V2F& left = v1;
	V2F& right = v2;
	if (v1.ScreenPos.x > v2.ScreenPos.x)
	{
		left = v2;
		right = v1;
	}
	int startY = int(v1.ScreenPos.y);
	int endY = int(v3.ScreenPos.y);
	int dy = endY - startY;
	for (int i = startY; i <= endY; ++i)
	{
		float weight = 0.0f;
		if(dy != 0)
			weight = float(i - startY) / float(dy);
		V2F newLeft = V2F::Lerp(left, v3, weight);
		V2F newRight = V2F::Lerp(right, v3, weight);
		newLeft.ScreenPos.y = i;
		newRight.ScreenPos.y = i;
		ScanLine(newLeft, newRight);
	}
}

void Rasterizer::ScanDownTriangle(V2F& v1, V2F& v2, V2F& v3)
{

}

void Rasterizer::ScanLine(const V2F& v1, const V2F& v2)
{
	int startX = int(v1.ScreenPos.x);
	int endX = int(v2.ScreenPos.x);
	int dx = endX - startX;
	for (int i = 0; i <= dx; ++i)
	{
		float weight = 0.0f;
		if(dx != 0)
			weight = float(i) / float(dx);
		V2F cur = V2F::Lerp(v1, v2, weight);
		m_window->DrawPixel(cur.ScreenPos.x, cur.ScreenPos.y, cur.Color);
	}
}