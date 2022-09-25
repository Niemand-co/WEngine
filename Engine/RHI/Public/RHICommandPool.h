#pragma once

class RHICommandBuffer;

class RHICommandPool
{
public:

	virtual ~RHICommandPool() = default;

	virtual RHICommandBuffer* GetCommandBuffer(unsigned int num = 1) = 0;

};