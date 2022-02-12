#pragma once
#include "Math/Vector.h"

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec3 color;
	Vec2 uv;

	Vertex() : position(Vec3()), normal(Vec3(0.0f, 1.0f, 0.0f)), color(Vec3()), uv(Vec2()){}
	Vertex(Vec3 pos, Vec3 norm = Vec3(0.0f, 1.0f, 0.0f), Vec3 color = Vec3(), Vec2 uv = Vec2()) : position(pos), normal(norm), color(color), uv(uv){}
	~Vertex() = default;
};

struct V2F
{
	Vec4 WorldPos;
	Vec4 ScreenPos;
	Vec3 Color;
	Vec3 Normal;
	Vec2 UV;

	V2F() = default;
	V2F(const Vec3& worldPos, const Vec3& screenPos, const Vec3& color, const Vec3& normal, const Vec2& uv)
		: WorldPos(worldPos), ScreenPos(screenPos), Color(color), Normal(normal), UV(uv){}
	~V2F() = default;

	static V2F Lerp(const V2F& v1, const V2F& v2, float factor)
	{
		V2F result;
		result.WorldPos = lerp(v1.WorldPos, v2.WorldPos, factor);
		result.ScreenPos = lerp(v1.ScreenPos, v2.ScreenPos, factor);
		result.Color = lerp(v1.Color, v2.Color, factor);
		result.Normal = lerp(v1.Normal, v2.Normal, factor);
		result.UV = lerp(v1.UV, v2.UV, factor);
		return result;
	}
};