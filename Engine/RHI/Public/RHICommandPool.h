#pragma once

class RHICommandPool : public RHIResource
{
public:

	virtual ~RHICommandPool() = default;

	virtual class RHICommandBuffer* GetCommandBuffer() = 0;

};