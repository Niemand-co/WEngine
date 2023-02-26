#pragma once

class RHICommandBuffer;

class RHICommandPool : public RHIResource
{
public:

	virtual ~RHICommandPool() = default;

	virtual RHICommandBuffer* GetCommandBuffer() = 0;

};