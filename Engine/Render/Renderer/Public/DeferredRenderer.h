#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

class DeferredRenderer : public SceneRenderer
{
public:

	DeferredRenderer();

	virtual ~DeferredRenderer();

	virtual void InitView();

	virtual void RenderPrePass();

	virtual void RenderBasePass();

	virtual void RenderShadowPass();

	virtual void RenderLightPass();

	virtual void RenderSkybox();

	virtual void RenderPostEffect();

private:

	void ComputeVisibility();

private:

	WEngine::CSMShadowMapPack *CSMMaps;

};