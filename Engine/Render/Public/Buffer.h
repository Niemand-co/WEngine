#pragma once

class WBuffer : public RenderResource
{
public:

	virtual ~WBuffer() = default;

};

class WVertexBuffer : public WBuffer
{
public:

	WVertexBuffer() = default;

	virtual ~WVertexBuffer() = default;

	WVertexBufferRHIRef& GetRHI() { return Buffer; }

	const WVertexBufferRHIRef& GetRHI() const { return Buffer; }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	WVertexBufferRHIRef Buffer;

};

typedef WEngine::WSharedPtr<WVertexBuffer> WVertexBufferRef;

class WIndexBuffer : public WBuffer
{
public:

	WIndexBuffer() = default;

	virtual ~WIndexBuffer() = default;

	WIndexBufferRHIRef& GetRHI() { return Buffer; }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	WIndexBufferRHIRef Buffer;

};

typedef WEngine::WSharedPtr<WIndexBuffer> WIndexBufferRef;

class WUniformBuffer : public WBuffer
{
public:

	WUniformBuffer() = delete;

	WUniformBuffer(const class ShaderParametersLayout* InLayout)
		: Layout(InLayout)
	{
	}

	virtual ~WUniformBuffer() = default;

	WUniformBufferRHIRef& GetRHI() { return Buffer; }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	const ShaderParametersLayout* Layout;

	WUniformBufferRHIRef Buffer;

};

typedef WEngine::WSharedPtr<WUniformBuffer> WUniformBufferRef;

class WDynamicUniform : public WUniformBuffer
{
public:

	WDynamicUniform() = default;

	virtual ~WDynamicUniform() = default;

};

typedef WEngine::WSharedPtr<WDynamicUniform> WDynamicUniformRef;