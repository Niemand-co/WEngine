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
				VulkanGroupPool& Pool = Pools[ID];
				return Pool.GetDescriptorSet();
			}
		}

	private:

		static WEngine::WHashMap<uint32, VulkanGroupPool> Pools;

	};

}