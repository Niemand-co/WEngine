#pragma once
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Public/CascadedShadowMap.h"

struct GBufferPack
{
	SRVTexture GBuffer0;
	SRVTexture GBuffer1;
	SRVTexture GBuffer2;
	SRVTexture GBuffer3;

	GBufferPack(unsigned int width, unsigned int height, Format format = Format::A32R32G32B32_SFloat)
		: GBuffer0(width, height, format),
		  GBuffer1(width, height, format),
		  GBuffer2(width, height, format),
		  GBuffer3(width, height, format)
	{
	}
};

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

	GBufferPack GBuffer;

	uint8 bRenderPrePass : 1;

	uint8 bUseSphereTest : 1;

	uint8 bUseBoxTest : 1;

	float MaxDrawDistance;

	class DeferredBasePass *BasePass;

};