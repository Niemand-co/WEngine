#pragma once

class RHIDevice;
class RHIDeviceDescriptor;

class RHIGPU
{
public:

	RHIGPU() = default;

	RHIGPU(const RHIGPU&) = delete;

	virtual ~RHIGPU() = 0;

	virtual const GPUFeature& GetFeature() const = 0;

	virtual const WEngine::WString& GetGPUName() const = 0;

	virtual RHIDevice* CreateDevice(RHIDeviceDescriptor *descriptor) = 0;

protected:
	
	WEngine::WString GPUName;

	GPUFeature m_feature;
	
};