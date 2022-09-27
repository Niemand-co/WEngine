#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"

namespace Vulkan
{

	VulkanSwapchain::VulkanSwapchain(VkSwapchainKHR* swapchain, VkImage* images, const unsigned int& imageCount, VkDevice *device, unsigned int familyIndex)
		: m_swapchain(swapchain), m_device(device), m_familyIndex(familyIndex)
	{
		m_textures.resize(imageCount);
		for (unsigned int i = 0; i < imageCount; ++i)
		{
			m_textures[i] = new VulkanTexture(images + i, m_device);
		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
	}

	void VulkanSwapchain::SetHandle(VkSwapchainKHR* swapchain)
	{
		m_swapchain = swapchain;
	}

	VkSwapchainKHR* VulkanSwapchain::GetHandle()
	{
		return m_swapchain;
	}

	unsigned int VulkanSwapchain::GetQueueFamilyIndex()
	{
		return m_familyIndex;
	}



}
