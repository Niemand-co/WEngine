#pragma once
#include "Render/Renderer.h"

class OGLRenderer : public Renderer
{
public:
	OGLRenderer(Primitive type);
	~OGLRenderer();

	virtual void DrawByIndices(Entity* entity, Light* light, Camera* camera) override;
private:

};