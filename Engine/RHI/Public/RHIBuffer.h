#pragma once

struct BufferResourceInfo;

class RHIBuffer
{
public:

	RHIBuffer();

	virtual ~RHIBuffer() = default;

	virtual void LoadData(void *pData, size_t size, size_t offset = 0) = 0;

	virtual void Flush(size_t range) = 0;

	virtual void Resize(size_t count) = 0;

	void SetDataSize(size_t size);

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