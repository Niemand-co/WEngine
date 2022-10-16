#pragma once

class RHIBuffer
{
public:

	virtual ~RHIBuffer() = default;

	virtual void LoadData(void *pData, size_t size) = 0;

public:

	unsigned int size;

};