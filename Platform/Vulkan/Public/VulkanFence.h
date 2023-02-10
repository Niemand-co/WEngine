#pragma once
#include "RHI/Public/RHIFence.h"

namespace Vulkan
{

	class VulkanFence : public RHIFence
	{
	public:

		VulkanFence(class VulkanDevice *pInDevice);

		virtual ~VulkanFence();

		virtual void Reset() override;

		virtual int32 Wait(double Time = UINT64_MAX) override;

		VkFence GetHandle() const { return Fence; }

	private:

		VkFence Fence;

		VulkanDevice *pDevice;

	};

}