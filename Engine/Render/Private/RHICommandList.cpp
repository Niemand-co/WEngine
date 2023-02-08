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

RHIRenderCommandList* GetRenderCommandList()
{
    static RHIRenderCommandList* list = new RHIRenderCommandList;
    return list;
}