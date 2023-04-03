#pragma once
#include "RHI/Public/RHIGroupPool.h"

namespace Vulkan
{

	class VulkanGroupPool : public RHIGroupPool
	{
	public:

		VulkanGroupPool(class VulkanDevice *pInDevice, VkDescriptorPoolCreateInfo* pInfo, VkDescriptorSetLayout InDescriptorSetLayout);

		virtual ~VulkanGroupPool();

		VkDescriptorSet GetDescriptorSet();

	private:

		VulkanDevice *pDevice;

		VkDescriptorSetLayout DescriptorSetLayout;

		VkDescriptorPool DescriptorSetPool;

	};

	class VulkanDescriptorSetManager : public RHIResource
	{
	public:

		static VkDescriptorSet GetDescriptorSet(uint32 ID)
		{
			if (Pools.Find(ID))
			{
				return Pools[ID]->GetDescriptorSet();
			}
			return VK_NULL_HANDLE;
		}

	private:

		static WEngine::WHashMap<uint32, VulkanGroupPool*> Pools;

	};

}