#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Public/Buffer.h"
#include "RHI/Public/RHIHeads.h"

RHIRenderCommandList* RHIRenderCommandList::g_instance = nullptr;

RHIRenderCommandList::RHIRenderCommandList()
{
}

void RHIRenderCommandList::BeginRenderPass(RHIRenderPassBeginDescriptor* descriptor)
{
}

void RHIRenderCommandList::EndRenderPass()
{
}

void RHIRenderCommandList::DrawIndexedPrimitive(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount)
{
}

uint32 RHIRenderCommandList::AcquireImageIndex()
{
    return RHIContext::GetContext()->AcquireImageIndex();
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

WTexture2DRHIRef RHIRenderCommandList::CreateTexture2D(uint32 InWidth, uint32 InHeight, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag)
{
    return RHIContext::GetContext()->CreateTexture2D(InWidth, InHeight, InFormat, InMipCount, InFlag);
}

WTexture2DArrayRHIRef RHIRenderCommandList::CreateTexture2DArray(uint32 InWith, uint32 InHeight, Format InFormat, uint32 InMipCount, uint32 InLayerCount, ETextureCreateFlags InFlag)
{
    return RHIContext::GetContext()->CreateTexture2DArray(InWith, InHeight, InFormat, InMipCount, InLayerCount, InFlag);
}

WTexture3DRHIRef RHIRenderCommandList::CreateTexture3D(uint32 InWith, uint32 InHeight, uint32 InDepth, Format InFormat, uint32 InMipCount, ETextureCreateFlags InFlag)
{
    return RHIContext::GetContext()->CreateTexture3D(InWith, InHeight, InDepth, InFormat, InMipCount, InFlag);
}

WRenderPassRHIRef RHIRenderCommandList::CreateRenderPass(RHIRenderPassDescriptor *descriptor)
{
    return RHIContext::GetContext()->CreateRenderPass(descriptor);
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}