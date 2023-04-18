#pragma once

struct BufferResourceInfo;
struct TextureResourceInfo;

class RHIBuffer : public RHIResource
{
public:

	RHIBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: Num(InNum), Stride(InStride), Usage(InUsage)
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


class RHIDynamicVertexBuffer : public RHIVertexBuffer
{
public:

	RHIDynamicVertexBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIVertexBuffer(InNum, InStride, InUsage)
	{
	}

	virtual ~RHIDynamicVertexBuffer() = default;

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


class RHIUniformBuffer : public RHIBuffer
{
public:

	RHIUniformBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIBuffer(InNum, InStride, InUsage)
	{
	}

	virtual ~RHIUniformBuffer() = default;

};


class RHIDynamicUniformBuffer : public RHIUniformBuffer
{
public:

	RHIDynamicUniformBuffer(uint32 InNum, uint32 InStride, EBufferUsageFlags InUsage)
		: RHIUniformBuffer(InNum, InStride, InUsage)
	{
	}

	virtual ~RHIDynamicUniformBuffer() = default;

};

class RHITextureBuffer : public RHIBuffer
{
public:

	virtual ~RHITextureBuffer() = default;

};