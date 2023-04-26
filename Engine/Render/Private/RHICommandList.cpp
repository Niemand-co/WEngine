#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/Buffer.h"
#include "RHI/Public/RHIHeads.h"

#define ALLOC_COMMAND(...) ::new (AllocCommand<__VA_ARGS__>()) __VA_ARGS__

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

void RHIRenderCommandList::SetGraphicsPipelineState(RHIGraphicsPipelineStateInitializer& GraphicsPipelineState)
{
    RHIGraphicsPipelineState *RHIPipelineState = CreateGraphicsPipelineState(GraphicsPipelineState);
    GetDynamicRHI()->RHISetGraphicsPipelineState(RHIPipelineState);
}

void RHIRenderCommandList::BindVertexBuffer(WVertexFactory* InVertexFactory)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandBindVertexBuffer)(InVertexFactory);
        return;
    }
    GetDynamicRHI()->RHIBindVertexBuffer(InVertexFactory);
}

void RHIRenderCommandList::SetStreamResource(const VertexInputStream& Stream)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandSetStreamResource)(Stream);
        return;
    }
    GetDynamicRHI()->RHISetStreamResource(Stream);
}

void RHIRenderCommandList::BindIndexBuffer(WIndexBufferRHIRef IndexBuffer)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandBindIndexBuffer)(IndexBuffer);
        return;
    }
    GetDynamicRHI()->RHIBindIndexBuffer(IndexBuffer);
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

void RHIRenderCommandList::SetScissor(int32 OffsetX, int32 OffsetY, uint32 Width, uint32 Height)
{
    if (IsOutOfRenderThread() && IsOutOfRHIThread())
    {
        ALLOC_COMMAND(RHICommandSetScissor)(OffsetX, OffsetY, Width, Height);
        return;
    }
    GetDynamicRHI()->RHISetScissor(OffsetX, OffsetY, Width, Height);
}

void RHIRenderCommandList::SetShaderUniformBuffer(RHIGraphicsShader* ShaderRHI, uint32 BufferIndex, WUniformBufferRHIRef UniformBuffer)
{
    if(IsOutOfRenderThread() && IsOutOfRHIThread())
    {
    }
    GetDynamicRHI()->RHISetShaderUniformBuffer(ShaderRHI, BufferIndex, UniformBuffer);
}

void RHIRenderCommandList::CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
{
    GetDynamicRHI()->RHICopyImageToBackBuffer(SrcTexture, DstTexture, SrcSizeX, SrcSizeY, DstSizeX, DstSizeY);
}

WVertexBufferRHIRef RHIRenderCommandList::CreateVertexBuffer(uint8* InContents, uint32 InStride, uint32 InCount, EBufferUsageFlags InUsage)
{
    if(InCount == 0)return nullptr;
    return GetDynamicRHI()->RHICreateVertexBuffer(InContents, InStride, InCount, InUsage);
}

WIndexBufferRHIRef RHIRenderCommandList::CreateIndexBuffer(uint8* InContents, uint32 InCount, EBufferUsageFlags InUsage)
{
    return GetDynamicRHI()->RHICreateIndexBuffer(InContents, InCount, InUsage);
}

WUniformBufferRHIRef RHIRenderCommandList::CreateUniformBuffer(uint8* InContents, const ShaderParametersLayout * InLayout, EUniformBufferUsage InUsage)
{
    return GetDynamicRHI()->RHICreateUniformBuffer(InContents, InLayout, InUsage);
}

WVertexShaderRHIRef RHIRenderCommandList::CreateVertexShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->RHICreateVertexShader(blob);
}

WPixelShaderRHIRef RHIRenderCommandList::CreatePixelShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->RHICreatePixelShader(blob);
}

WGeometryShaderRHIRef RHIRenderCommandList::CreateGeometryShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->RHICreateGeometryShader(blob);
}

WComputeShaderRHIRef RHIRenderCommandList::CreateComputeShader(ShaderCodeBlob& blob)
{
    return GetDynamicRHI()->RHICreateComputeShader(blob);
}

WTextureRHIRef RHIRenderCommandList::CreateTexture(const RHITextureDesc& InDesc)
{
    return GetDynamicRHI()->RHICreateTexture(InDesc);
}
}

WTextureViewRHIRef RHIRenderCommandList::CreateTextureView(uint32 InMipIndex, uint32 InMipCount, uint32 InLayerIndex, uint32 InLayerCount, uint32 InPlaneIndex, uint32 InPlaneCount, EDimension InDimension, EFormat InFormat, RHITexture* InTexture)
{
    return GetDynamicRHI()->CreateTextureView(InMipIndex, InMipCount, InLayerIndex, InLayerCount, InPlaneIndex, InPlaneCount, InDimension, InFormat, InTexture);
}

WViewportRHIRef RHIRenderCommandList::CreateViewport(uint32 InWidth, uint32 InHeight, bool bInFullScreen, EFormat InFormat)
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

WSamplerStateRHIRef RHIRenderCommandList::CreateSamplerState(const RHISamplerStateInitializer& Initialzer)
{
    return GetDynamicRHI()->RHICreateSamplerState(Initializer);
}

WVertexInputStateRHIRef RHIRenderCommandList::GetOrCreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements)
{
    return GetStaticRHI()->GetOrCreateVertexInputState(InElements);
}

void RHIRenderCommandList::UpdateUniformBuffer(WUniformBufferRHIRef UniformBuffer, void* Contents)
{
    GetDynamicRHI()->UpdateUniformBuffer(UniformBuffer, Contents);
}

RHIGraphicsPipelineState* RHIRenderCommandList::CreateGraphicsPipelineState(RHIGraphicsPipelineStateInitializer& Initializer)
{
    return GetDynamicRHI()->RHICreateGraphicsPipelineState(Initializer);
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}