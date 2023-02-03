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
	pBuffer = (RHIBuffer*)RHIContext::GetContext()->CreateVertexBuffer(Stride, Count);
}

void WVertexBuffer::ReleaseRHIResource()
{
	pBuffer = nullptr;
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
	pBuffer = (RHIBuffer*)RHIContext::GetContext()->CreateIndexBuffer(Count);
}

void WIndexBuffer::ReleaseRHIResource()
{
	pBuffer = nullptr;
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
	pBuffer = (RHIBuffer*)RHIContext::GetContext()->CreateUniformBuffer(Stride, Count);
}

void WUniformBuffer::ReleaseRHIResource()
{
	pBuffer = nullptr;
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
