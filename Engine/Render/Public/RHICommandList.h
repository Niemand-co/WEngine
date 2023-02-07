#pragma once

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

	class WVertexBuffer* CreateVertexBuffer(size_t stride, size_t count);

	class WIndexBuffer* CreateIndexBuffer(size_t count);

	class WUniformBuffer* CreateUniformBuffer(size_t stride, size_t count);

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