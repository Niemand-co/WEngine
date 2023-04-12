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
    GetDynamicRHI()->RHIBeginDrawingViewport(Viewport);
}

void RHIRenderCommandList::EndDrawingViewport(RHIViewport* Viewport, bool bPresent)
{
    GetDynamicRHI()->RHIEndDrawingViewport(Viewport, bPresent);
}

WRenderPassRHIRef RHIRenderCommandList::BeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor)
{
    return GetDynamicRHI()->RHIBeginRenderPass(RenderPasDescriptor, FramebufferDescriptor);
}

void RHIRenderCommandList::EndRenderPass()
{
    GetDynamicRHI()->RHIEndRenderPass();
}

void RHIRenderCommandList::BeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions)
{
    GetDynamicRHI()->RHIBeginTransition(Transitions);
}

void RHIRenderCommandList::SetGraphicsPipelineState(RHIGraphicsPipelineStateDescriptor* GraphicsPipelineState)
{
    GetDynamicRHI()->RHISetGraphicsPipelineState(GraphicsPipelineState);
}

void RHIRenderCommandList::DrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandDrawIndexedPrimitive)(indexCount, firstIndex, instanceCount);
        return;
    }
    GetDynamicRHI()->RHIDrawIndexedPrimitive(indexCount, firstIndex, instanceCount);
}

void RHIRenderCommandList::SetViewport(float MinX, float MinY, float MaxX, float MaxY, float MinDepth, float MaxDepth)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandSetViewport)(MinX, MaxX, MinY, MaxY, MinDepth, MaxDepth);
        return;
    }
    GetDynamicRHI()->RHISetViewport(MinX, MinY, MaxX - MinX, MaxY - MinY, MinDepth, MaxDepth);
}

void RHIRenderCommandList::CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
{
    GetDynamicRHI()->CopyImageToBackBuffer(SrcTexture, DstTexture, SrcSizeX, SrcSizeY, DstSizeX, DstSizeY);
}

WVertexBufferRHIRef RHIRenderCommandList::CreateVertexBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
    if(InCount == 0)return nullptr;
    return GetDynamicRHI()->CreateVertexBuffer(InContents, InStride, InCount, InUsage);
}

WIndexBufferRHIRef RHIRenderCommandList::CreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage)
{
    return GetDynamicRHI()->CreateIndexBuffer(InContents, InCount, InUsage);
}

WUniformBufferRHIRef RHIRenderCommandList::CreateUniformBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
    return GetDynamicRHI()->CreateUniformBuffer(InContents, InStride, InCount, InUsage);
}

WVertexShaderRHIRef RHIRenderCommandList::CreateVertexShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->CreateVertexShader(blob);
}

WPixelShaderRHIRef RHIRenderCommandList::CreatePixelShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->CreatePixelShader(blob);
}

WGeometryShaderRHIRef RHIRenderCommandList::CreateGeometryShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->CreateGeometryShader(blob);
}

WComputeShaderRHIRef RHIRenderCommandList::CreateComputeShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->CreateComputeShader(blob);
}

WTexture2DRHIRef RHIRenderCommandList::CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return GetDynamicRHI()->CreateTexture2D(InWidth, InHeight, InFormat, InMipCount, InClearValue, InFlag, InitState);
}

WTexture2DArrayRHIRef RHIRenderCommandList::CreateTexture2DArray(uint32 InWith, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return GetDynamicRHI()->CreateTexture2DArray(InWith, InHeight, InFormat, InMipCount, InLayerCount, InClearValue, InFlag, InitState);
}

WTexture3DRHIRef RHIRenderCommandList::CreateTexture3D(uint32 InWith, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ClearValue InClearValue, ETextureCreateFlags InFlag, EAccess InitState)
{
    return GetDynamicRHI()->CreateTexture3D(InWith, InHeight, InDepth, InFormat, InMipCount, InClearValue, InFlag, InitState);
}

WTextureViewRHIRef RHIRenderCommandList::CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, Dimension InDimension, Format InFormat, RHITexture* InTexture)
{
    return GetDynamicRHI()->CreateTextureView(InMipIndex, InMipCount, InLayerIndex, InLayerCount, InPlaneIndex, InPlaneCount, InDimension, InFormat, InTexture);
}

WViewportRHIRef RHIRenderCommandList::CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, Format InFormat)
{
    return GetDynamicRHI()->CreateViewport(InWidth, InHeight, bInFullScreen, InFormat);
}

WBlendStateRHIRef RHIRenderCommandList::CreateBlendState(const RHIBlendStateInitializer& Initializer)
{
    return GetStaticRHI()->CreateBlendState(Initializer);
}

WDepthStencilStateRHIRef RHIRenderCommandList::CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer)
{
    return GetStaticRHI()->CreateDepthStencilState(Initializer);
}

WRasterizationStateRHIRef RHIRenderCommandList::CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer)
{
    return GetStaticRHI()->CreateRasterizationState(Initializer);
}

WMultiSampleStateRHIRef RHIRenderCommandList::CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer)
{
    return GetStaticRHI()->CreateMultiSampleState(Initializer);
}

WVertexInputStateRHIRef RHIRenderCommandList::CreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements)
{
    return GetStaticRHI()->CreateVertexInputState(InElements);
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}