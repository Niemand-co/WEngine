#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

class DeferredRenderer : public SceneRenderer
{
public:

	DeferredRenderer(CameraComponent *pCamera, class WViewport* Viewport);

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

	void FrustumCulling(const WEngine::WArray<class PrimitiveInfo*>& Primitives);

	void OcclusionCulling(const WEngine::WArray<class PrimitiveInfo*>& Primitives);

	void SetupBasePass(class WViewport *Viewport);

private:

	CSMShadowMapPack CSMMaps;

	uint8 bRenderPrePass : 1;

	uint8 bUseSphereTest : 1;

	uint8 bUseBoxTest : 1;

	float MaxDrawDistance;

	class WRDGBuilder* GraphBuilder;

};