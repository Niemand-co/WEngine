#pragma once
#include "RHI/Public/RHICore.h"

enum class EAccess : uint16;

class RHIContext
{
public:

	RHIContext() = default;

	virtual ~RHIContext() = default;

};

class StaticRHIContext : public RHIContext
{
public:

	StaticRHIContext() = default;

	~StaticRHIContext() = default;

	virtual WAttachmentBlendStateRHIRef CreateBlendState(const RHIBlendStateInitializer& Initializer) = 0;

	virtual WDepthStencilStateRHIRef CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer) = 0;

	virtual WRasterizationStateRHIRef CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer) = 0;

	virtual WMultiSampleStateRHIRef CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer) = 0;

	virtual WVertexInputStateRHIRef GetOrCreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements) = 0;

};

class DynamicRHIContext : public RHIContext
{
public:

	DynamicRHIContext();

	virtual ~DynamicRHIContext();

	WVertexBufferRHIRef RHICreateVertexBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage);

	WIndexBufferRHIRef RHICreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage);

	WUniformBufferRHIRef RHICreateUniformBuffer(uint8* InContents, const class ShaderParametersLayout* InLayout, EUniformBufferUsage InUsage);

	WVertexShaderRHIRef RHICreateVertexShader(class ShaderCodeBlob& blob);

	WPixelShaderRHIRef RHICreatePixelShader(class ShaderCodeBlob& blob);

	WGeometryShaderRHIRef RHICreateGeometryShader(class ShaderCodeBlob& blob);

	WComputeShaderRHIRef RHICreateComputeShader(class ShaderCodeBlob& blob);

	WTextureRHIRef RHICreateTexture(const class RHITextureDesc& InDesc);

	WTextureViewRHIRef RHICreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, EDimension InDimension, EFormat InFormat, class RHITexture* InTexture);

	virtual RHIPipelineStateObject* RHICreateGraphicsPipelineState(class RHIGraphicsPipelineStateInitializer& Initializer) = 0;

	virtual void RHICopyBufferToImage(class RHITexture* pTexture, class RHIBuffer* pBuffer, unsigned int width, unsigned int height);

	virtual void RHIUpdateUniformBuffer(WUniformBufferRHIRef UniformBuffer, void *Contents) = 0;

	virtual WViewportRHIRef RHICreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, EFormat InFormat);

	virtual void RHIBeginDrawingViewport(class RHIViewport* Viewport) = 0;

	virtual void RHIEndDrawingViewport(class RHIViewport* Viewport, bool bPresent) = 0;

	virtual WRenderPassRHIRef RHIBeginRenderPass(class RHIRenderPassDescriptor* RenderPasDescriptor, class RHIFramebufferDescriptor* FramebufferDescriptor) = 0;

	virtual void RHIEndRenderPass() = 0;

	virtual void RHISetViewport(float MinX, float MinY, float MaxX, float MaxY, float MinZ, float MaxZ) = 0;

	virtual void RHISetScissor(uint32 MinX, uint32 MinY, uint32 Width, uint32 Height) = 0;

	virtual void RHIBindVertexBuffer(class WVertexFactory* InVertexFactory) = 0;

	virtual void RHISetStreamResource(uint32 StreamIndex, WVertexBufferRHIRef Buffer, uint32 Offset) = 0;

	virtual void RHIBindIndexBuffer(WIndexBufferRHIRef InIndexBuffer) = 0;

	virtual void RHIDrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount) = 0;

	virtual void RHIBeginTransition(WEngine::WArray<class RHIBarrierDescriptor>& Transitions) = 0;

	virtual void RHICopyImageToBackBuffer(class RHITexture* SrcTexture, WTextureRHIRef DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY) = 0;

	virtual void RHISetGraphicsPipelineState(class RHIGraphicsPipelineState *GraphicsPipelineState) = 0;

	virtual RHIRenderPass* RHICreateRenderPass(const class RHIGraphicsPipelineStateInitializer& Initializer) = 0;

	virtual void RHISetShaderUniformBuffer(class RHIGraphicsShader *ShaderRHI, uint32 BufferIndex, WUniformBufferRHIRef UniformBuffer) = 0;

	virtual WSamplerStateRHIRef RHICreateSamplerState(const RHISamplerStateInitializer& Initializer) = 0;

	inline class RHIInstance* GetInstance() const { return pInstance; }

	class RHIGPU* GetGPU() const;

	inline class RHIDevice* GetDevice() { return pDevice; }

	inline class RHIQueue* GetQueue() { return pQueue; }

protected:

	class RHIInstance* pInstance;

	class RHIDevice* pDevice;

	class RHIQueue* pQueue;

};

StaticRHIContext* GetStaticRHI();

DynamicRHIContext* GetDynamicRHI();