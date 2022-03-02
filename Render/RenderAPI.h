#pragma once
#include "Math/Vector.h"

class RenderAPI
{
public:
	enum class API
	{
		None = 0,
		OpenGL
	};

	virtual void Init() = 0;
	virtual void SetClearColor(Vec3 Color) = 0;
	virtual void SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height) = 0;

	inline static API GetAPI() { return s_api; }

private:
	static API s_api;
};