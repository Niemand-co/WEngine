#pragma once
#include "RHI/Public/RHICore.h"
#include "Render/Public/RenderDependencyGraphDefinitions.h"
#include "Utils/Public/ShaderCodeBlob.h"

class RenderTarget;
class RHIGraphicsEncoder;

class RHICommandListBase
{
public:

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

	const class RHIContext* GetContext() const { return m_pContext; }

	void Execute();

	void* AllocCommand(int32 InSize, int32 InAlignment);

	template<typename T>
	void* AllocCommand()
	{
		return AllocCommand(sizeof(T), alignof(T));
	}

protected:

	virtual ~RHICommandListBase() = default;

protected:

	RHIContext *m_pContext;

	struct RHICommandBase *Root         = nullptr;
	struct RHICommandBase **CommandLink = nullptr;

};

class RHIRenderCommandList : public RHICommandListBase
{
public:

	RHIRenderCommandList();

	virtual ~RHIRenderCommandList() = default;

	void BeginDrawingViewport(RHIViewport *Viewport);

	void EndDrawingViewport(RHIViewport *Viewport, bool bPresent);

	WRenderPassRHIRef BeginRenderPass(class RHIRenderPassDescriptor *RenderPasDescriptor, class RHIFramebufferDescriptor *FramebufferDescriptor);

	void EndRenderPass();

	void BeginTransition(WEngine::WArray<class RHIBarrierDescriptor>& Transitions);

	void SetGraphicsPipelineState(class RHIGraphicsPipelineStateDescriptor* GraphicsPipelineState);

	void BindVertexBuffer(class WVertexFactory* InVertexFactory);

	void SetStreamResource(const VertexInputStream& Stream);

	void BindIndexBuffer(WIndexBufferRHIRef IndexBuffer);

	void DrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount);

	void SetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth);

	void SetScissor(int32 OffsetX, int32 OffsetY, uint32 Width, uint32 Height);

	void CopyImageToBackBuffer(class RHITexture *SrcTexture, class RHITexture *DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY);

	WVertexBufferRHIRef CreateVertexBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage);

	WIndexBufferRHIRef CreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage);

	WUniformBufferRHIRef CreateUniformBuffer(uint8* InContents, const class ShaderParametersLayout * InLayout, EBufferUsageFlags InUsage);

	WVertexShaderRHIRef CreateVertexShader(ShaderCodeBlob& blob);

	WPixelShaderRHIRef CreatePixelShader(ShaderCodeBlob& blob);

	WGeometryShaderRHIRef CreateGeometryShader(ShaderCodeBlob& blob);

	WComputeShaderRHIRef CreateComputeShader(ShaderCodeBlob& blob);

	WTextureRHIRef CreateTexture(const class RHITextureDesc& InDesc);

	WTextureViewRHIRef CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, EDimension InDimension, EFormat InFormat, class RHITexture* InTexture);

	WViewportRHIRef CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, EFormat InFormat);

	WBlendStateRHIRef CreateBlendState(const RHIBlendStateInitializer& Initializer);

	WDepthStencilStateRHIRef CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer);

	WRasterizationStateRHIRef CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer);

	WMultiSampleStateRHIRef CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer);

	WVertexInputStateRHIRef GetOrCreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements);

	void UpdateUniformBuffer(WUniformBufferRHIRef UniformBuffer, void *Contents);

};

class RHIComputeCommandList : public RHICommandListBase
{
public:

	RHIComputeCommandList();

	virtual ~RHIComputeCommandList() = default;

};

RHIRenderCommandList* GetRenderCommandList();