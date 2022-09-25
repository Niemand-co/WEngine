#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"

namespace Vulkan
{

	VulkanSwapchain::VulkanSwapchain(VkSwapchainKHR* swapchain, VkImage* images, const unsigned int& imageCount, VkDevice *device)
		: m_swapchain(swapchain), m_device(device)
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

	VkSwapchainKHR* VulkanSwapchain::GetHandle()
	{
		return m_swapchain;
	}



}
