#pragma once

class WBuffer : public RenderResource
{
public:

	virtual ~WBuffer() = default;

protected:

	WEngine::WSharedPtr<class RHIBuffer> pBuffer;

};

class WVertexBuffer : public WBuffer
{
public:

	WVertexBuffer(size_t inStride, size_t inCount);

	virtual ~WVertexBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	size_t Stride;

	size_t Count;

};

class WIndexBuffer : public WBuffer
{
public:

	WIndexBuffer(size_t inCount);

	virtual ~WIndexBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	size_t Count;

};

class WUniformBuffer : public WBuffer
{
public:

	WUniformBuffer(size_t inStride, size_t inCount);

	virtual ~WUniformBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	size_t Stride;

	size_t Count;

};

class WDynamicUniform : public WUniformBuffer
{
public:

	WDynamicUniform(size_t inStride, size_t inCount);

	virtual ~WDynamicUniform();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;
};