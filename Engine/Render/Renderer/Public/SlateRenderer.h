#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"

class SlateRenderer : public ScriptableRenderer
{
public:

	SlateRenderer(class CameraComponent *pInCamera);

	virtual ~SlateRenderer();

	virtual void Render() override;

};