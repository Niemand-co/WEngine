#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

class WRDGBuilder* GetRDGBuilder();

class DeferredRenderer : public SceneRenderer
{
public:

	DeferredRenderer(const WSceneViewFamily *InViewFamily);

	virtual ~DeferredRenderer();

	virtual void Render(WRDGBuilder& GraphBuilder) override;

	void RenderPrePass(WRDGBuilder& GraphBuilder, WViewInfo &View);

	void RenderBasePass(WRDGBuilder& GraphBuilder, WViewInfo& View);

	void RenderShadowPass(WRDGBuilder& GraphBuilder, WViewInfo& View);

	void RenderLightPass(WRDGBuilder& GraphBuilder, WViewInfo& View);

	void RenderSkybox(WRDGBuilder& GraphBuilder, WViewInfo& View);

	void RenderTranslucent(WRDGBuilder& GraphBuilder, WViewInfo& View);

	void RenderPostEffect(WRDGBuilder& GraphBuilder, WViewInfo& View);

private:

	CSMShadowMapPack CSMMaps;

	uint8 bRenderPrePass : 1;

	float MaxDrawDistance;

};