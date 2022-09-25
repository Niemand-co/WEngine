#pragma once

class RenderAPI
{
public:
	enum class API
	{
		None = 0,
		OpenGL,
		Vulkan
	};

	virtual void Init() = 0;
	virtual void SetClearColor(Vector4 Color) = 0;
	virtual void Clear() = 0;
	virtual void SetViewport(const uint32_t& x, const uint32_t& y, const uint32_t& width, const uint32_t& height) = 0;

	inline static API GetAPI() { return s_api; }

private:
	static API s_api;
};