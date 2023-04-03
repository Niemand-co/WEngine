#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"

class SlateRenderer : public ScriptableRenderer
{
public:

	SlateRenderer();

	virtual ~SlateRenderer();

	virtual void Render() override;

private:

	glm::mat4 ViewMatrix;

	WEngine::WSharedPtr<class RScene> ActiveScene;

};