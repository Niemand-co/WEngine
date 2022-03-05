#pragma once
#ifndef SR_RENDERER_H
#define SR_RENDERER_H

#include "Scene/Entity.h"
#include <vector>

class Light;
class Camera;

class Renderer
{
public:
	enum class Primitive
	{
		TRIANGLE = 0,
		LINE,
		POINT
	};

	virtual ~Renderer() = default;

	virtual void DrawByIndices(Entity* entity, Light* light, Camera* camera) = 0;
protected:
	Primitive m_type;
};

class RendererLibrary
{
public:
	static Renderer* Allocate(Renderer::Primitive type);
private:
	static std::vector<Renderer*> m_renderers;
};

#endif