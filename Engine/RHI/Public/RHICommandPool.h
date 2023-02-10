#pragma once

class RHICommandBuffer;

class RHICommandPool : public RHIResource
{
public:

	virtual ~RHICommandPool() = default;

	virtual RHICommandBuffer* GetCommandBuffer(bool isPrimary = true) = 0;

};