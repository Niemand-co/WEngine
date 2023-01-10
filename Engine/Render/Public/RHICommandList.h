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

	const RHIContext* GetContext() const { return m_pContext; }

protected:

	virtual ~RHICommandListBase() = default;

protected:

	class RHIContext *m_pContext;

};

class RHIRenderCommandList : public RHICommandListBase
{
public:

	RHIRenderCommandList();

	virtual ~RHIRenderCommandList() = default;

	WEngine::WSharedPtr<RHIGraphicsEncoder> GetEncoder() const { return m_pEncoder; }

	void BeginRenderPass(class RHIRenderPassBeginDescriptor *descriptor);

	void EndRenderPass();

	void DrawIndexedPrimitive(unsigned int indexCount, unsigned int firstIndex, unsigned int instanceCount);

protected:

	WEngine::WSharedPtr<class RHIGraphicsEncoder> m_pEncoder;

};

class RHIComputeCommandList : public RHICommandListBase
{
public:

	RHIComputeCommandList();

	virtual ~RHIComputeCommandList() = default;

	const RHIComputeEncoder* GetEncoder() const { return m_pEncoder; }

protected:

	class RHIComputeEncoder *m_pEncoder;

};