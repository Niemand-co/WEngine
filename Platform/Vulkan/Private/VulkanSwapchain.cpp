#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Render/Public/RenderTarget.h"

namespace Vulkan
{

	VulkanSwapchain::VulkanSwapchain(VkSwapchainKHR* swapchain, VkDevice *device, unsigned int familyIndex)
		: pSwapchain(swapchain), m_pDevice(device), m_familyIndex(familyIndex), pDevice(device)
	{
		unsigned int imageCount = 0;
		vkGetSwapchainImagesKHR(*m_pDevice, *pSwapchain, &imageCount, nullptr);
		VkImage *pImages = (VkImage*)WEngine::Allocator::Get()->Allocate(imageCount * sizeof(VkImage));
		vkGetSwapchainImagesKHR(*m_pDevice, *pSwapchain, &imageCount, pImages);

		RenderTargets.Resize(imageCount);
		for (unsigned int i = 0; i < imageCount; ++i)
		{
			RenderTargets[i] = new  WRenderTarget();
		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(*m_pDevice, *pSwapchain, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());
		for (int i = 0; i < RenderTargets.Size(); ++i)
		{
			BeginReleaseResource(RenderTargets[i]);
		}
	}

	int32 VulkanSwapchain::AcquireImageIndex(RHISemaphore** outSemaphore)
	{
		//vkAcquireNextImageKHR(*pDevice, *pSwapchain, VK_TIMEOUT, );
		return 0;
	}

	void VulkanSwapchain::Present(RHIQueue* queue, RHISemaphore* renderingDoneSemaphore)
	{
	}

	void VulkanSwapchain::SetHandle(VkSwapchainKHR* pSwapchain)
	{
		pSwapchain = pSwapchain;
	}

	VkSwapchainKHR* VulkanSwapchain::GetHandle()
	{
		return pSwapchain;
	}

	unsigned int VulkanSwapchain::GetQueueFamilyIndex()
	{
		return m_familyIndex;
	}



}
