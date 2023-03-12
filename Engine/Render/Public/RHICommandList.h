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

protected:

	virtual ~RHICommandListBase() = default;

protected:

	RHIContext *m_pContext;

};

class RHIRenderCommandList : public RHICommandListBase
{
public:

	RHIRenderCommandList();

	virtual ~RHIRenderCommandList() = default;

	RHIGraphicsEncoder* GetEncoder() const { return m_pEncoder; }

	void BeginDrawingViewport(RHIViewport *Viewport);

	void EndDrawingViewport(RHIViewport *Viewport, bool bPresent);

	void BeginRenderPass(class RHIRenderPassDescriptor *RenderPasDescriptor, class RHIFramebufferDescriptor *FramebufferDescriptor);

	void EndRenderPass();

	void BeginTransition(WEngine::WArray<class RHIBarrierDescriptor>& Transitions);

	void SetGraphicsPipelineState(class RHIGraphicsPipelineStateDescriptor* GraphicsPipelineState);

	void DrawIndexedPrimitive(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount);

	void SetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth);

	void CopyImageToBackBuffer(class RHITexture *SrcTexture, class RHITexture *DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY);

	WVertexBufferRHIRef CreateVertexBuffer(uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage);

	WIndexBufferRHIRef CreateIndexBuffer(uint32 InCount, EBufferUsageFlags InUsage);

	WUniformBufferRHIRef CreateUniformBuffer(uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage);

	WVertexShaderRHIRef CreateVertexShader(ShaderCodeBlob& blob);

	WPixelShaderRHIRef CreatePixelShader(ShaderCodeBlob& blob);

	WGeometryShaderRHIRef CreateGeometryShader(ShaderCodeBlob& blob);

	WComputeShaderRHIRef CreateComputeShader(ShaderCodeBlob& blob);

	WTexture2DRHIRef CreateTexture2D(uint32 InWith, uint32 InHeight, Format InFormat, uint32 InMipCount = 1u, ClearValue InClearValue = { 0, 0, 0, 0 }, ETextureCreateFlags InFlag = ETextureCreateFlags::TextureCreate_None, EAccess InitState = EAccess::Unknown);

	WTexture2DArrayRHIRef CreateTexture2DArray(uint32 InWith, uint32 InHeight, Format InFormat, uint32 InMipCount = 1u, uint32 InLayerCount = 1u, ClearValue InClearValue = { 0, 0, 0, 0 }, ETextureCreateFlags InFlag = ETextureCreateFlags::TextureCreate_None, EAccess InitState = EAccess::Unknown);

	WTexture3DRHIRef CreateTexture3D(uint32 InWith, uint32 InHeight, uint32 InDepth, Format format, uint32 InMipCount = 1u, ClearValue InClearValue = { 0, 0, 0, 0 }, ETextureCreateFlags InFlag = ETextureCreateFlags::TextureCreate_None, EAccess InitState = EAccess::Unknown);

	WTextureViewRHIRef CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, class RHITexture* InTexture);

	WViewportRHIRef CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, Format InFormat);

public:

	static RHIRenderCommandList* Get() { if(g_instance == nullptr)g_instance = new RHIRenderCommandList(); return g_instance; }

private:

	static RHIRenderCommandList* g_instance;

protected:

	class RHIGraphicsEncoder *m_pEncoder;

};

class RHIComputeCommandList : public RHICommandListBase
{
public:

	RHIComputeCommandList();

	virtual ~RHIComputeCommandList() = default;

	class RHIComputeEncoder* GetEncoder() const { return m_pEncoder; }

protected:

	RHIComputeEncoder *m_pEncoder;

};

RHIRenderCommandList* GetRenderCommandList();