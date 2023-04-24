#pragma once

class RHIBuffer : public RHIResource
{
public:

	RHIBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIResource(ERHIResourceType::RRT_Buffer), Num(InNum), Stride(InStride), Usage(InUsage)
	{
	}

	virtual ~RHIBuffer() = default;

	virtual void* GetBufferRHIBase() = 0;

	uint32 GetSize() const { return Num * Stride; }

protected:

	uint32 Num;

	uint32 Stride;

	EBufferUsageFlags Usage;

};

class RHIVertexBuffer : public RHIBuffer
{
public:

	RHIVertexBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIBuffer(InNum, InStride, InUsage)
	{
	}

	virtual ~RHIVertexBuffer() = default;

};

class RHIIndexBuffer : public RHIBuffer
{
public:

	RHIIndexBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIBuffer(InNum, InStride, InUsage)
	{
	}

	virtual ~RHIIndexBuffer() = default;

};


class RHIUniformBuffer : public RHIResource
{
public:

	RHIUniformBuffer(const class ShaderParametersLayout* InLayout)
		: RHIResource(ERHIResourceType::RRT_Buffer)
	{
	}

	virtual ~RHIUniformBuffer() = default;

};

class RHITextureBuffer : public RHIBuffer
{
public:

	virtual ~RHITextureBuffer() = default;

};