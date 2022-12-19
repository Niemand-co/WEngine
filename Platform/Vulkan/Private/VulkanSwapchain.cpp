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
		VkImage *pImages = (VkImage*)WEngine::Allocator::Get()->Allocate(imageCount * sizeof(VkImage));
		vkGetSwapchainImagesKHR(*m_pDevice, *m_pSwapchain, &imageCount, pImages);

		m_textures.Resize(imageCount);
		for (unsigned int i = 0; i < imageCount; ++i)
		{
			m_textures[i] = (RHITexture*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanAllocator));
			::new (m_textures[i]) VulkanTexture(pImages + i, nullptr, nullptr, m_pDevice, true);
		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(*m_pDevice, *m_pSwapchain, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		for (int i = 0; i < m_textures.Size(); ++i)
		{
			m_textures[i]->~RHITexture();
			WEngine::Allocator::Get()->Deallocate(m_textures[i]);
		}
	}

	void VulkanSwapchain::SetHandle(VkSwapchainKHR* pSwapchain)
	{
		m_pSwapchain = pSwapchain;
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
