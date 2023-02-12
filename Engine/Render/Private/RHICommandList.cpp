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
    return return RHIContext::GetContext->AcquireImageIndex();
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

WTexture2DRHIRef RHIRenderCommandList::CreateTexture2D(uint32 width, uint32 height, Format format, uint32 usage, uint32 aspect)
{
    return RHIContext::GetContext()->CreateTexture2D(width, height, format, usage, aspect);
}

WTexture2DArrayRHIRef RHIRenderCommandList::CreateTexture2DArray(uint32 width, uint32 height, Format format, uint32 layerCount, uint32 usage, uint32 aspect)
{
    return RHIContext::GetContext()->CreateTexture2DArray(width, height, format, layerCount, usage, aspect);
}

WTexture3DRHIRef RHIRenderCommandList::CreateTexture3D(uint32 width, uint32 height, uint32 depth, Format format, uint32 usage, uint32 aspect)
{
    return RHIContext::GetContext()->CreateTexture3D(width, height, depth, format, usage, aspect);
}

WRenderPassRHIRef RHIRenderCommandList::CreateRenderPass(RHIRenderPassDescriptor *descriptor)
{
    return RHIContext::GetContext()->CreateRenderPass();
}

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}