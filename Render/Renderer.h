#pragma once
#ifndef SR_RENDERER_H
#define SR_RENDERER_H

#include "Scene/Entity.h"
#include "Render/Rasterizer.h"
#include "Utils/Shader.h"
#include "Scene/Light.h"
#include "Camera/Camera.h"
#include <vector>

class Renderer
{
public:
	enum class Primitive
	{
		TRIANGLE = 0,
		LINE,
		POINT
	};

	Renderer(Primitive type);
	~Renderer() = default;

	void DrawByIndices(Entity* entity, Light* light, Camera* camera);
private:
	Rasterizer* m_rasterizer;
	Primitive m_type;
};

class RendererLibrary
{
public:
	static Renderer* Allocate(Renderer::Primitive type);
private:
	static std::vector<Renderer> m_renderers;
};

#endif