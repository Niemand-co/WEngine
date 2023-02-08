#include "pch.h"
#include "Render/Public/Buffer.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "RHI/Public/RHIContext.h"

WVertexBuffer::WVertexBuffer(size_t inStride, size_t inCount)
	: Stride(inStride), Count(inCount)
{
}

WVertexBuffer::~WVertexBuffer()
{
}

void WVertexBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateVertexBuffer(Stride, Count);
}

void WVertexBuffer::ReleaseRHIResource()
{
	Buffer = nullptr;
}

void WVertexBuffer::UpdateRHIResource()
{
}

WIndexBuffer::WIndexBuffer(size_t inCount)
	: Count(inCount)
{
}

WIndexBuffer::~WIndexBuffer()
{
}

void WIndexBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateIndexBuffer(Count);
}

void WIndexBuffer::ReleaseRHIResource()
{
	Buffer = nullptr;
}

void WIndexBuffer::UpdateRHIResource()
{
}

WUniformBuffer::WUniformBuffer(size_t inStride, size_t inCount)
	: Stride(inStride), Count(inCount)
{
}

WUniformBuffer::~WUniformBuffer()
{
}

void WUniformBuffer::InitRHIResource()
{
	Buffer = GetRenderCommandList()->CreateUniformBuffer(Stride, Count);
}

void WUniformBuffer::ReleaseRHIResource()
{
	Buffer = nullptr;
}

void WUniformBuffer::UpdateRHIResource()
{
}

WDynamicUniform::WDynamicUniform(size_t inStride, size_t inCount)
	: WUniformBuffer(inStride, inCount)
{
}

WDynamicUniform::~WDynamicUniform()
{
}

void WDynamicUniform::InitRHIResource()
{
}

void WDynamicUniform::ReleaseRHIResource()
{
}

void WDynamicUniform::UpdateRHIResource()
{
}
