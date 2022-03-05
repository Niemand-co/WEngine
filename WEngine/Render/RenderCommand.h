#pragma once
#include "Math/Vector.h"
#include "Platform/Windows/WindowsRenderAPI.h"

class RenderCommand
{
public:
	static void Init();

	static void SetClearColor(Vec4 Color);

	static void Clear();

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void BeginScene();

	static void EndScene();
private:
	static RenderAPI* m_renderAPI;
};