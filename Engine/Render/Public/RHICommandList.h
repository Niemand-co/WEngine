#pragma once
#include "RHI/Public/RHICore.h"
#include "Utils/Public/ShaderCodeBlob.h"

class RenderTarget;
class RHIGraphicsEncoder;

class RHICommandListBase
{
public:

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

	const class RHIContext* GetContext() const { return m_pContext; }

protected:

	virtual ~RHICommandListBase() = default;

protected:

	RHIContext *m_pContext;

};

class RHIRenderCommandList : public RHICommandListBase
{
public:

	RHIRenderCommandList();

	virtual ~RHIRenderCommandList() = default;

	RHIGraphicsEncoder* GetEncoder() const { return m_pEncoder; }

	void BeginRenderPass(class RHIRenderPassBeginDescriptor *descriptor);

	void EndRenderPass();

	void DrawIndexedPrimitive(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount);

	uint32 AcquireImageIndex();

	WVertexBufferRHIRef CreateVertexBuffer(size_t stride, size_t count);

	WIndexBufferRHIRef CreateIndexBuffer(size_t count);

	WUniformBufferRHIRef CreateUniformBuffer(size_t stride, size_t count);

	WVertexShaderRHIRef CreateVertexShader(ShaderCodeBlob& blob);

	WPixelShaderRHIRef CreatePixelShader(ShaderCodeBlob& blob);

	WGeometryShaderRHIRef CreateGeometryShader(ShaderCodeBlob& blob);

	WComputeShaderRHIRef CreateComputeShader(ShaderCodeBlob& blob);

	WTexture2DRHIRef CreateTexture2D(uint32 width, uint32 height, Format format, uint32 usage = IMAGE_USAGE_COLOR_ATTACHMENT, uint32 aspect = IMAGE_ASPECT_COLOR);

	WTexture2DArrayRHIRef CreateTexture2DArray(uint32 width, uint32 height, Format format, uint32 layerCount, uint32 usage = IMAGE_USAGE_COLOR_ATTACHMENT, uint32 aspect = IMAGE_ASPECT_COLOR);

	WTexture3DRHIRef CreateTexture3D(uint32 width, uint32 height, uint32 depth, Format format, uint32 usage = IMAGE_USAGE_COLOR_ATTACHMENT, uint32 aspect = IMAGE_ASPECT_COLOR);

	WRenderPassRHIRef CreateRenderPass(class RHIRenderPassDescriptor* descriptor);

public:

	static RHIRenderCommandList* Get() { if(g_instance == nullptr)g_instance = new RHIRenderCommandList(); return g_instance; }

private:

	static RHIRenderCommandList* g_instance;

protected:

	class RHIGraphicsEncoder *m_pEncoder;

};

class RHIComputeCommandList : public RHICommandListBase
{
public:

	RHIComputeCommandList();

	virtual ~RHIComputeCommandList() = default;

	class RHIComputeEncoder* GetEncoder() const { return m_pEncoder; }

protected:

	RHIComputeEncoder *m_pEncoder;

};

RHIRenderCommandList* GetRenderCommandList();