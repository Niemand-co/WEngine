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

	WVertexBufferRHIRef GetRHI() { return Buffer.Get(); }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	WEngine::WSharedPtr<RHIVertexBuffer> Buffer;

};

typedef WEngine::WSharedPtr<WVertexBuffer> WVertexBufferRef;

class WIndexBuffer : public WBuffer
{
public:

	WIndexBuffer() = default;

	virtual ~WIndexBuffer() = default;

	WIndexBufferRHIRef GetRHI() { return Buffer.Get(); }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	WEngine::WSharedPtr<RHIIndexBuffer> Buffer;

};

typedef WEngine::WSharedPtr<WIndexBuffer> WIndexBufferRef;

class WUniformBuffer : public WBuffer
{
public:

	WUniformBuffer() = delete;

	WUniformBuffer(const ShaderParametersLayout* InLayout)
		: Layout(InLayout)
	{
	}

	virtual ~WUniformBuffer() = default;

	WUniformBufferRHIRef GetRHI() { return Buffer.Get(); }

	virtual void ReleaseRHIResource() override
	{
		Buffer = nullptr;
	}

protected:

	const WShaderParameterLayout* Layout;

	WEngine::WSharedPtr<RHIUniformBuffer> Buffer;

};

typedef WEngine::WSharedPtr<WUniformBuffer> WUniformBufferRef;

class WDynamicUniform : public WUniformBuffer
{
public:

	WDynamicUniform() = default;

	virtual ~WDynamicUniform() = default;

};

typedef WEngine::WSharedPtr<WDynamicUniform> WDynamicUniformRef;