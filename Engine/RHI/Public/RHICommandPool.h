#pragma once

class RHICommandBuffer;

class RHICommandPool
{
public:

	virtual ~RHICommandPool() = default;

	virtual RHICommandBuffer* GetCommandBuffer(bool isPrimary = true) = 0;

	virtual std::vector<RHICommandBuffer*> GetCommandBuffer(unsigned int num, bool isPrimary = true) = 0;

};