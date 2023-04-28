#pragma once

class RHICommandBuffer;

class RHICommandPool : public RHIResource
{
public:

	RHICommandPool()
		: RHIResource(ERHIResourceType::RRT_CommandBufferPool)
	{
	}

	virtual ~RHICommandPool() = default;

	virtual RHICommandBuffer* GetCommandBuffer() = 0;

};