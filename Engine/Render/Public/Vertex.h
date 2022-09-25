#pragma once

template<typename T>
struct Vector4Type;
template<typename T>
struct Vector3Type;
template<typename T>
struct Vector2Type;

typedef Vector4Type<float> Vector4;
typedef Vector3Type<float> Vector3;
typedef Vector2Type<float> Vector2;

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	Vector2 uv;

	Vertex(){}
	Vertex(Vector3 pos, Vector3 norm = Vector3(0.0f, 1.0f, 0.0f), Vector3 color = Vector3(), Vector2 uv = Vector2()) : position(pos), normal(norm), color(color), uv(uv){}
	~Vertex() = default;
};

struct V2F
{
	Vector4 WorldPos;
	Vector4 ScreenPos;
	Vector3 Color;
	Vector3 Normal;
	Vector2 UV;

	V2F() = default;
	V2F(const Vector3& worldPos, const Vector3& screenPos, const Vector3& color, const Vector3& normal, const Vector2& uv)
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