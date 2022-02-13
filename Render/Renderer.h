#pragma once
#ifndef SR_RENDERER_H
#define SR_RENDERER_H

#include "Utils/Shader.h"
#include "Scene/Entity.h"
#include "Render/Rasterizer.h"
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

	void SetShader(Shader* shader);

	void DrawByIndices(Entity* entity);
private:
	Shader* m_shader;
	Rasterizer* m_rasterizer;
	Primitive m_type;
};

#endif