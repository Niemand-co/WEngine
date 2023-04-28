#pragma once
#include "RHI/Public/RHIResource.h"

class RHIInstanceDescriptor;
class RHIGPU;

class RHIInstance : public RHIResource
{
public:

	RHIInstance()
		: RHIResource(ERHIResourceType::RRT_Instance)
	{
	}

	RHIInstance(const RHIInstance&) = delete;

	virtual ~RHIInstance() = default;

	static RHIInstance* CreateInstance(RHIInstanceDescriptor *descriptor);

	virtual void InitializeGPU() = 0;

	virtual RHIGPU* GetGPU(unsigned int gpuID);

public:

	static RHIInstance* g_instance;

protected:
	
	WEngine::WArray<RHIGPU*> m_gpus;

};