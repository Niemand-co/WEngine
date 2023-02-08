#pragma once

class WBuffer : public RenderResource
{
public:

	virtual ~WBuffer() = default;

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

	WVertexBufferRHIRef Buffer;

	size_t Stride;

	size_t Count;

};

typedef WEngine::WSharedPtr<WVertexBuffer> WVertexBufferRef;

class WIndexBuffer : public WBuffer
{
public:

	WIndexBuffer(size_t inCount);

	virtual ~WIndexBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	WIndexBufferRHIRef Buffer;

	size_t Count;

};

typedef WEngine::WSharedPtr<WIndexBuffer> WIndexBufferRef;

class WUniformBuffer : public WBuffer
{
public:

	WUniformBuffer(size_t inStride, size_t inCount);

	virtual ~WUniformBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	WUniformBufferRHIRef Buffer;

	size_t Stride;

	size_t Count;

};

typedef WEngine::WSharedPtr<WUniformBuffer> WUniformBufferRef;

class WDynamicUniform : public WUniformBuffer
{
public:

	WDynamicUniform(size_t inStride, size_t inCount);

	virtual ~WDynamicUniform();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	WDynamicUniformBufferRHIRef Buffer;

};

typedef WEngine::WSharedPtr<WDynamicUniform> WDynamicUniformRef;