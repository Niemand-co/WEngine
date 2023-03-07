#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Public/Buffer.h"
#include "RHI/Public/RHIHeads.h"

RHIRenderCommandList* RHIRenderCommandList::g_instance = nullptr;

RHIRenderCommandList::RHIRenderCommandList()
{
}

void RHIRenderCommandList::BeginDrawingViewport(RHIViewport* Viewport)
{
    RHIContext::GetContext()->RHIBeginDrawingViewport(Viewport);
}

void RHIRenderCommandList::EndDrawingViewport(RHIViewport* Viewport, bool bPresent)
{
    RHIContext::GetContext()->RHIEndDrawingViewport(Viewport, bPresent);
}

void RHIRenderCommandList::BeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor)
{
    RHIContext::GetContext()->RHIBeginRenderPass(RenderPasDescriptor, FramebufferDescriptor);
}

void RHIRenderCommandList::EndRenderPass()
{
    RHIContext::GetContext()->RHIEndRenderPass();
}

void RHIRenderCommandList::BeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions)
{
    RHIContext::GetContext()->RHIBeginTransition(Transitions);
}

void RHIRenderCommandList::SetGraphicsPipelineState(RHIGraphicsPipelineStateDescriptor* GraphicsPipelineState)
{
    RHIContext::GetContext()->RHISetGraphicsPipelineState(GraphicsPipelineState);
}

void RHIRenderCommandList::DrawIndexedPrimitive(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount)
{
}

void RHIRenderCommandList::SetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth)
{
}

void RHIRenderCommandList::CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
{
    RHIContext::GetContext()->CopyImageToBackBuffer(SrcTexture, DstTexture, SrcSizeX, SrcSizeY, DstSizeX, DstSizeY);
}

WVertexBufferRHIRef RHIRenderCommandList::CreateVertexBuffer(size_t stride, size_t count)
{
    return RHIContext::GetContext()->CreateVertexBuffer(stride, count);
}

WIndexBufferRHIRef RHIRenderCommandList::CreateIndexBuffer(size_t count)
{
    return RHIContext::GetContext()->CreateIndexBuffer(count);
}

WUniformBufferRHIRef RHIRenderCommandList::CreateUniformBuffer(size_t stride, size_t count)
{
    return RHIContext::GetContext()->CreateUniformBuffer(stride, count);
}

WVertexShaderRHIRef RHIRenderCommandList::CreateVertexShader(ShaderCodeBlob& blob)
{
    return RHIContext::GetContext()->CreateVertexShader(blob);
}

WPixelShaderRHIRef RHIRenderCommandList::CreatePixelShader(ShaderCodeBlob& blob)
{
    return RHIContext::GetContext()->CreatePixelShader(blob);
}

WGeometryShaderRHIRef RHIRenderCommandList::CreateGeometryShader(ShaderCodeBlob& blob)
{
    return RHIContext::GetContext()->CreateGeometryShader(blob);
}

WComputeShaderRHIRef RHIRenderCommandList::CreateComputeShader(ShaderCodeBlob& blob)
{
    return RHIContext::GetContext()->CreateComputeShader(blob);
}

WTexture2DRHIRef RHIRenderCommandList::CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return RHIContext::GetContext()->CreateTexture2D(InWidth, InHeight, InFormat, InMipCount, InClearValue, InFlag, InitState);
}

WTexture2DArrayRHIRef RHIRenderCommandList::CreateTexture2DArray(uint32 InWith, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return RHIContext::GetContext()->CreateTexture2DArray(InWith, InHeight, InFormat, InMipCount, InLayerCount, InClearValue, InFlag, InitState);
}

WTexture3DRHIRef RHIRenderCommandList::CreateTexture3D(uint32 InWith, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return RHIContext::GetContext()->CreateTexture3D(InWith, InHeight, InDepth, InFormat, InMipCount, InClearValue, InFlag, InitState);
}

WTextureViewRHIRef RHIRenderCommandList::CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture)
{
    return RHIContext::GetContext()->CreateTextureView(InMipIndex, InMipCount, InLayerIndex, InLayerCount, InPlaneIndex, InPlaneCount, InDimension, InFormat, InTexture);
}

WViewportRHIRef RHIRenderCommandList::CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, Format InFormat)
{
    return RHIContext::GetContext()->CreateViewport(InWidth, InHeight, bInFullScreen, InFormat);
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}