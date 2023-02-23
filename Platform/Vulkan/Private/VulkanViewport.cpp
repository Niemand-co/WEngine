#include "pch.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanView.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{

	VulkanViewport::VulkanViewport(VulkanDevice *pInDevice)
		: pDevice(pInDevice)
	{
		
	}

	VulkanViewport::~VulkanViewport()
	{
		
	}

	void VulkanViewport::Tick()
	{
	}

	uint32 VulkanViewport::AcquireImageIndex()
	{
		return uint32();
	}

	void VulkanViewport::Present()
	{
	}

	void VulkanViewport::CreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
		pSwapchain = static_cast<VulkanSwapchain*>(pDevice->CreateSwapchain(descriptor));
	}

	void VulkanViewport::DestroySwapchain()
	{
	}

	void VulkanViewport::RecreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
	}

}