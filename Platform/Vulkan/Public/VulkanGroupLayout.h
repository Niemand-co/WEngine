#pragma once
#include "RHI/Public/RHIGroupLayout.h"

namespace Vulkan
{

	class VulkanDescriptorSetLayout : public RHIDescriptorSetLayout
	{
	public:

		VulkanDescriptorSetLayout(class VulkanDevice* pInDevice, VkDescriptorSetLayoutCreateInfo *pInfo);

		virtual ~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout GetHandle() const { return DescriptorSetLayout; }

	private:

		VulkanDevice *pDevice;

		VkDescriptorSetLayout DescriptorSetLayout;

		WEngine::WArray<VkDescriptorSetLayoutBinding> Bindings;

	};

	class VulkanDescriptorSetLayoutManager : public RHIResource
	{
	public:

		static VulkanDescriptorSetLayout* GetDescriptorSetLayout(uint32 ID)
		{
			if (Layouts.Find(ID))
			{
				return Layouts[ID];
			}
			return nullptr;
		}

		static void AddDescriptorLayout(uint32 ID, VulkanDescriptorSetLayout* Layout)
		{
			Layouts[ID] = Layout;
		}

	private:

		static WEngine::WHashMap<uint32, VulkanDescriptorSetLayout*> Layouts;

	};

}