#include "pch.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Render/Public/RenderTarget.h"

namespace Vulkan
{

	VulkanSwapchain::VulkanSwapchain(VulkanDevice* pInDevice, VulkanInstance* pInInstance, VkSwapchainCreateInfoKHR *pInfo)
		: pDevice(pInDevice), pInstance(pInInstance)
	{
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(0);
		surfaceCreateInfo.hwnd = glfwGetWin32Window((GLFWwindow*)Window::Get()->GetHandle());

		auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(pInstance->GetHandle(), "vkCreateWin32SurfaceKHR");

		RE_ASSERT(CreateWin32SurfaceKHR(pInstance->GetHandle(), &surfaceCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Surface) == VK_SUCCESS, "Failed to Create Win32 Surface.");

		pInfo->surface = Surface;

		VulkanGPU *gpu = pDevice->GetGPU();

		/** Check the format for presenting. */
		{
			uint32 FormatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(*gpu->GetHandle(), Surface, &FormatCount, nullptr);
			RE_ASSERT(FormatCount > 0, "No Format Support On This Device.");
			WEngine::WArray<VkSurfaceFormatKHR> Formats(FormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(*gpu->GetHandle(), Surface, &FormatCount, Formats.GetData());

			bool bFound = false;
			for (VkSurfaceFormatKHR& format : Formats)
			{
				if (pInfo->imageFormat == format.format)
				{
					if (pInfo->imageColorSpace == format.colorSpace)
					{
						bFound = true;
						break;
					}
				}
			}

			if (!bFound)
			{
				pInfo->imageFormat = Formats[0].format;
				pInfo->imageColorSpace = Formats[0].colorSpace;
			}
		}

		/** Get surface capabilities support on this device. */
		{
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*gpu->GetHandle(), Surface, &SurfaceCapabilities);
			pInfo->imageExtent.width = pInfo->imageExtent.width < SurfaceCapabilities.minImageExtent.width ? SurfaceCapabilities.minImageExtent.width : pInfo->imageExtent.width;
			pInfo->imageExtent.height = pInfo->imageExtent.height < SurfaceCapabilities.minImageExtent.height ? SurfaceCapabilities.minImageExtent.height : pInfo->imageExtent.height;
			pInfo->imageExtent.width = pInfo->imageExtent.width > SurfaceCapabilities.maxImageExtent.width ? SurfaceCapabilities.maxImageExtent.width : pInfo->imageExtent.width;
			pInfo->imageExtent.height = pInfo->imageExtent.height > SurfaceCapabilities.maxImageExtent.height ? SurfaceCapabilities.maxImageExtent.height : pInfo->imageExtent.height;
		}

		/** Check the present mode support on this device */
		{
			uint32 PresentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(*gpu->GetHandle(), Surface, &PresentModeCount, nullptr);
			RE_ASSERT(PresentModeCount > 0, "This device doesn't supporting presenting.");
			WEngine::WArray<VkPresentModeKHR> PresentModes(PresentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(*gpu->GetHandle(), Surface, &PresentModeCount, PresentModes.GetData());
			bool bSupportMailbox = false;
			bool bSupportImmediate = false;
			bool bSupportFIFO = false;
			for (VkPresentModeKHR& PresentMode : PresentModes)
			{
				switch (PresentMode)
				{
				case VK_PRESENT_MODE_MAILBOX_KHR:
					bSupportMailbox = true;
					break;
				case VK_PRESENT_MODE_IMMEDIATE_KHR:
					bSupportImmediate = true;
					break;
				case VK_PRESENT_MODE_FIFO_KHR:
					bSupportFIFO = true;
					break;
				default:
					break;
				}
			}
			pInfo->presentMode = bSupportMailbox ? VK_PRESENT_MODE_MAILBOX_KHR : ( bSupportImmediate ? VK_PRESENT_MODE_IMMEDIATE_KHR : ( bSupportFIFO ? VK_PRESENT_MODE_FIFO_KHR : PresentModes[0] ) );
		}

		/** Check the min image count. */
		{
			if (SurfaceCapabilities.maxImageCount > 0)
			{
				pInfo->minImageCount = pInfo->minImageCount < SurfaceCapabilities.minImageCount ? SurfaceCapabilities.minImageCount : pInfo->minImageCount;
				pInfo->minImageCount = pInfo->minImageCount > SurfaceCapabilities.maxImageCount ? SurfaceCapabilities.maxImageCount : pInfo->minImageCount;
			}
		}

		pInfo->preTransform = SurfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ? VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR : SurfaceCapabilities.currentTransform;
		pInfo->compositeAlpha = SurfaceCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR : VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		pInfo->clipped = VK_TRUE;
		pInfo->oldSwapchain = VK_NULL_HANDLE;

		vkCreateSwapchainKHR(pInDevice->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Swapchain);

		RecreateInfo = *pInfo;

		ImageAcquireSemaphore.Reserve(pInfo->minImageCount);
		ImageAcquireFence.Reserve(pInfo->minImageCount);
		for (uint32 ImageIndex = 0; ImageIndex < pInfo->minImageCount; ++ImageIndex)
		{
			ImageAcquireSemaphore.Push(new VulkanSemaphore(pDevice));
			ImageAcquireFence.Push(new VulkanFence(pDevice));
		}
		
		PresentID = 0;
		SemaphoreID = 0;
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(pDevice->GetHandle(), Swapchain, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
		vkDestroySurfaceKHR(pInstance->GetHandle(), Surface, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	int32 VulkanSwapchain::AcquireImageIndex(RHISemaphore** OutSemaphore)
	{
		uint32 ImageIndex = 0;
		const uint32 PreSemaphoreID = SemaphoreID;
		SemaphoreID = (SemaphoreID + 1) % ImageAcquireSemaphore.Size();

#if VULKAN_USE_FENCE_ACQUIRE_IMAGE
		ImageAcquireFence[SemaphoreID]->Reset();
		VkFence Fence = static_cast<VulkanFence*>(ImageAcquireFence[SemaphoreID].Get())->GetHandle();
#else
		VkFence Fence = VK_NULL_HANDLE;
#endif

		VkResult Result = vkAcquireNextImageKHR(pDevice->GetHandle(), Swapchain, VK_TIMEOUT, static_cast<VulkanSemaphore*>(ImageAcquireSemaphore[SemaphoreID].Get())->GetHandle(), Fence, &ImageIndex);

		if (Result = VK_ERROR_OUT_OF_DATE_KHR)
		{
			SemaphoreID = PreSemaphoreID;
			return (int32)EState::OutofData;
		}

		if (Result = VK_ERROR_SURFACE_LOST_KHR)
		{
			SemaphoreID = PreSemaphoreID;
			return (int32)EState::SurfaceLost;
		}

		*OutSemaphore = ImageAcquireSemaphore[SemaphoreID].Get();

#if VULKAN_USE_FENCE_ACQUIRE_IMAGE
		ImageAcquireFence[SemaphoreID]->Wait();
#endif

		CurrentImageIndex = (int32)ImageIndex;
		return CurrentImageIndex;
	}

	int32 VulkanSwapchain::Present(RHIQueue* Queue, RHISemaphore* RenderingDoneSemaphore)
	{
		VkSemaphore Semaphore;
		if(RenderingDoneSemaphore)
			Semaphore = static_cast<VulkanSemaphore*>(RenderingDoneSemaphore)->GetHandle();
		else
			Semaphore = VK_NULL_HANDLE;

		VkPresentInfoKHR Info = {};
		{
			Info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			Info.swapchainCount = 1;
			Info.pSwapchains = &Swapchain;
			Info.waitSemaphoreCount = 1;
			Info.pWaitSemaphores = &Semaphore;
			Info.pImageIndices = (const uint32*)(&CurrentImageIndex);
		}

		VkResult Result = vkQueuePresentKHR(static_cast<VulkanQueue*>(Queue)->GetHandle(), &Info);

		if (Result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			return (int32)EState::OutofData;
		}

		if (Result == VK_ERROR_SURFACE_LOST_KHR)
		{
			return (int32)EState::SurfaceLost;
		}

		return (int32)EState::Healthy;
	}

}
