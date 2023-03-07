#pragma once
#include "RHI/Public/RHIGroupPool.h"

namespace Vulkan
{

	class VulkanGroupPool : public RHIGroupPool
	{
	public:

		VulkanGroupPool(class VulkanDevice *pInDevice, VkDescriptorPoolCreateInfo* pInfo);

		virtual ~VulkanGroupPool();

		virtual WEngine::WArray<RHIGroup*> GetGroup(unsigned int count = 1) override;

	private:

		VulkanDevice *pDevice;

		VkDescriptorPool DescriptorSetPool;

	};

}