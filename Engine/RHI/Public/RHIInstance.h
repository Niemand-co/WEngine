#pragma once
#ifndef WENGINE_RHI_H
#define WENGINE_RHI_H

class RHIInstanceDescriptor;
class RHIGPU;
class RHISurface;

class RHIInstance
{
public:

	RHIInstance() = default;

	RHIInstance(const RHIInstance&) = delete;

	virtual ~RHIInstance() = default;

	static RHIInstance* CreateInstance(RHIInstanceDescriptor *descriptor);

	virtual void InitializeGPU() = 0;

	virtual RHIGPU* GetGPU(unsigned int gpuID);

	virtual RHISurface* GetSurface();

	virtual void UpdateSurface() = 0;

public:

	static RHIInstance* g_instance;

protected:
	
	std::vector<RHIGPU*> m_gpus;

	RHISurface* m_surface;

};

#endif