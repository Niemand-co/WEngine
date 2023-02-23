#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

class DeferredRenderer : public SceneRenderer
{
public:

	DeferredRenderer(CameraComponent *pCamera);

	virtual ~DeferredRenderer();

	virtual void Render() override;

	void InitView();

	void RenderPrePass();

	void RenderBasePass();

	void RenderShadowPass();

	void RenderLightPass();

	void RenderSkybox();

	void RenderTranslucent();

	void RenderPostEffect();

private:

	void ComputeVisibility();

private:

	CSMShadowMapPack CSMMaps;

	uint8 bRenderPrePass : 1;

	uint8 bUseSphereTest : 1;

	uint8 bUseBoxTest : 1;

	float MaxDrawDistance;

	class WRDGBuilder* GraphBuilder;

};