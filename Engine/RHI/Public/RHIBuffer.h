#pragma once

struct BufferResourceInfo;
struct TextureResourceInfo;

class RHIBuffer : public RHIResource
{
public:

	RHIBuffer();

	virtual ~RHIBuffer() = default;

	virtual void LoadData(void *pData, size_t size, size_t offset = 0) = 0;

	virtual void Flush(size_t range) = 0;

	virtual void Resize(size_t count) = 0;

	void SetDataSize(size_t size);

	inline size_t Size() const { return m_size; }

	inline size_t Alignment() const { return m_dynamicAlignment; }

	inline bool IsDynamic() { return m_isDynamic; }

	BufferResourceInfo* GetBufferInfo();

protected:

	size_t m_size;

	size_t m_capacity;

	bool m_isDynamic;

	size_t m_dataSize;

	size_t m_dynamicAlignment;

	BufferResourceInfo *m_pInfos;

};

class RHIVertexBuffer : public RHIBuffer
{
public:

	virtual ~RHIVertexBuffer() = default;

};


class RHIDynamicVertexBuffer : public RHIBuffer
{
public:

	virtual ~RHIDynamicVertexBuffer() = default;

};


class RHIIndexBuffer : public RHIBuffer
{
public:

	virtual ~RHIIndexBuffer() = default;

};


class RHIUniformBuffer : public RHIBuffer
{
public:

	virtual ~RHIUniformBuffer() = default;

};


class RHIDynamicUniformBuffer : public RHIUniformBuffer
{
public:

	virtual ~RHIDynamicUniformBuffer() = default;

};

class RHITextureBuffer : public RHIBuffer
{
public:

	virtual ~RHITextureBuffer() = default;

};