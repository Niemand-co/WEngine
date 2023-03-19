#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/Buffer.h"
#include "RHI/Public/RHIHeads.h"

#define ALLOC_COMMAND(...) ::new (AllocCommand<__VA_ARGS__>()) __VA_ARGS__

RHIRenderCommandList* RHIRenderCommandList::g_instance = nullptr;

void RHICommandListBase::Execute()
{
    while (*CommandLink != nullptr)
    {
        RHICommandBase *Command = *CommandLink;
        CommandLink = &Command->next;
        Command->ExecuteAndDestruct(this);
    }
}

void* RHICommandListBase::AllocCommand(int32 InSize, int32 InAlignment)
{
    RHICommandBase *Result = (RHICommandBase*)NormalAllocator::Get()->Allocate(InSize);
    *CommandLink = Result;
    CommandLink = &Result->next;
    return Result;
}

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

WRenderPassRHIRef RHIRenderCommandList::BeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor)
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

void RHIRenderCommandList::DrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandDrawIndexedPrimitive)(indexCount, firstIndex, instanceCount);
        return;
    }
    RHIContext::GetContext()->RHIDrawIndexedPrimitive(indexCount, firstIndex, instanceCount);
}

void RHIRenderCommandList::SetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandSetViewport)(X, X + Width, Y, Y + Height, MinDepth, MaxDepth);
        return;
    }
    RHIContext::GetContext()->RHISetViewport(X, Y, Width, Height, MinDepth, MaxDepth);
}

void RHIRenderCommandList::CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
{
    RHIContext::GetContext()->CopyImageToBackBuffer(SrcTexture, DstTexture, SrcSizeX, SrcSizeY, DstSizeX, DstSizeY);
}

WVertexBufferRHIRef RHIRenderCommandList::CreateVertexBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
    return RHIContext::GetContext()->CreateVertexBuffer(InContents, InStride, InCount, InUsage);
}

WIndexBufferRHIRef RHIRenderCommandList::CreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage)
{
    return RHIContext::GetContext()->CreateIndexBuffer(InContents, InCount, InUsage);
}

WUniformBufferRHIRef RHIRenderCommandList::CreateUniformBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
    return RHIContext::GetContext()->CreateUniformBuffer(InContents, InStride, InCount, InUsage);
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

WBlendStateRHIRef RHIRenderCommandList::CreateBlendState(const RHIBlendStateInitializer& Initializer)
{
    return RHIContext::GetContext()->CreateBlendState(Initializer);
}

WDepthStencilStateRHIRef RHIRenderCommandList::CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer)
{
    return RHIContext::GetContext()->CreateDepthStencilState(Initializer);
}

WRasterizationStateRHIRef RHIRenderCommandList::CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer)
{
    return RHIContext::GetContext()->CreateRasterizationState(Initializer);
}

WMultiSampleStateRHIRef RHIRenderCommandList::CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer)
{
    return RHIContext::GetContext()->CreateMultiSampleState(Initializer);
}

WVertexInputStateRHIRef RHIRenderCommandList::CreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements)
{
    return RHIContext::GetContext()->CreateVertexInputState(InElements);
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}