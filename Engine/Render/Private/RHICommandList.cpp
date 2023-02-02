#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Render/Public/Buffer.h"

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

WVertexBuffer* RHIRenderCommandList::CreateVertexBuffer(size_t stride, size_t count)
{
    return new WVertexBuffer();
}

WIndexBuffer* RHIRenderCommandList::CreateIndexBuffer(size_t stride, size_t count)
{
    return nullptr;
}

WUniformBuffer* RHIRenderCommandList::CreateUniformBuffer(size_t stride, size_t count)
{
    return nullptr;
}
