#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"

namespace Vulkan
{

	VulkanSwapchain::VulkanSwapchain(VkSwapchainKHR* swapchain, VkDevice *device, unsigned int familyIndex)
		: m_pSwapchain(swapchain), m_pDevice(device), m_familyIndex(familyIndex)
	{
		unsigned int imageCount = 0;
		vkGetSwapchainImagesKHR(*m_pDevice, *m_pSwapchain, &imageCount, nullptr);
		m_images.resize(imageCount);
		vkGetSwapchainImagesKHR(*m_pDevice, *m_pSwapchain, &imageCount, m_images.data());

		m_textures.resize(imageCount);
		for (unsigned int i = 0; i < imageCount; ++i)
		{
			m_textures[i] = (RHITexture*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanAllocator));
			::new (m_textures[i]) VulkanTexture(&m_images[i], m_pDevice, true);
		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(*m_pDevice, *m_pSwapchain, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		for (int i = 0; i < m_textures.size(); ++i)
		{
			m_textures[i]->~RHITexture();
			WEngine::Allocator::Get()->Deallocate(m_textures[i]);
		}
		m_images.clear();
	}

	void VulkanSwapchain::SetHandle(VkSwapchainKHR* pSwapchain)
	{
		m_pSwapchain = pSwapchain;
	}

	void VulkanSwapchain::UpdateTexture()
	{
		unsigned int imageCount = 0;
		vkGetSwapchainImagesKHR(*m_pDevice, *m_pSwapchain, &imageCount, nullptr);
		m_images.clear();
		m_images.resize(imageCount);
		vkGetSwapchainImagesKHR(*m_pDevice, *m_pSwapchain, &imageCount, m_images.data());

		for (int i = 0; i < m_textures.size(); ++i)
		{
			static_cast<VulkanTexture*>(m_textures[i])->SetHandle(&m_images[i]);
		}
	}

	VkSwapchainKHR* VulkanSwapchain::GetHandle()
	{
		return m_pSwapchain;
	}

	unsigned int VulkanSwapchain::GetQueueFamilyIndex()
	{
		return m_familyIndex;
	}



}
