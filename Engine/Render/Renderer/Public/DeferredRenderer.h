#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

class WRDGBuilder* GetRDGBuilder();

class DeferredRenderer : public SceneRenderer
{
public:

	DeferredRenderer(const WSceneViewFamily *InViewFamily);

	virtual ~DeferredRenderer();

	virtual void Render() override;

	void InitView();

	void RenderPrePass(WViewInfo &View);

	void RenderBasePass(WViewInfo& View);

	void RenderShadowPass(WViewInfo& View);

	void RenderLightPass(WViewInfo& View);

	void RenderSkybox(WViewInfo& View);

	void RenderTranslucent(WViewInfo& View);

	void RenderPostEffect(WViewInfo& View);

private:

	CSMShadowMapPack CSMMaps;

	uint8 bRenderPrePass : 1;

	float MaxDrawDistance;

	class WRDGBuilder* GraphBuilder;

};