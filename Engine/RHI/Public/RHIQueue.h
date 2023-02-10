#pragma once

class RHIQueue : public RHIResource
{
public:
	
	virtual ~RHIQueue() = default;

	virtual int GetIndex() = 0;

	virtual class RHICommandPool* GetCommandPool() = 0;

};