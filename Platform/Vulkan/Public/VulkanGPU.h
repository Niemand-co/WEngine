#pragma once
#include "RHI/Public/RHIGPU.h"

class RHIDeviceDescriptor;
class RHIDecvice;

namespace Vulkan
{

	class VulkanGPU : public RHIGPU
	{
	public:

		VulkanGPU() = delete;

		VulkanGPU(VkPhysicalDevice *pPhysicalDevice, VkSurfaceKHR *surface);

		virtual ~VulkanGPU();

		virtual const GPUFeature& GetFeature() const override;

		virtual const WEngine::WString& GetGPUName() const override;

		virtual RHIDevice* CreateDevice(RHIDeviceDescriptor *descriptor) override;

		VkPhysicalDevice* GetHandle();

	private:

		VkPhysicalDevice *m_pPhysicalDevice;

	};

}