#pragma once

class RHIBuffer
{
public:

	virtual ~RHIBuffer() = default;

	virtual void LoadData(void *pData, size_t size, size_t offset = 0) = 0;

	virtual void Flush(size_t range) = 0;

public:

	unsigned int size;

};