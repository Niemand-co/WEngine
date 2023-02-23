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
		int32 NewImageIndex = pSwapchain->AcquireImageIndex(&pAcquireImageSemaphore);
		if (NewImageIndex != -1)
		{
			AcquiredImageIndex = NewImageIndex;
		}
		else
		{

		}

	}

	void VulkanViewport::Present()
	{
		AcquireImageIndex();
	}

	void VulkanViewport::CreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
		pSwapchain = static_cast<VulkanSwapchain*>(pDevice->CreateSwapchain(descriptor, BackBufferImages));

		RenderingDoneSemaphores.Resize(BackBufferImages.Size());
		TextureViews.Resize(BackBufferImages.Size());

		RHITextureViewDescriptor TextureViewDescriptor = {};
		{
			TextureViewDescriptor.format = PixelFormat;
			TextureViewDescriptor.dimension = Dimension::Texture2D;
			TextureViewDescriptor.baseMipLevel = 0;
			TextureViewDescriptor.mipCount = 1;
			TextureViewDescriptor.baseArrayLayer = 0;
			TextureViewDescriptor.arrayLayerCount = 1;
			TextureViewDescriptor.planeIndex = 0;
			TextureViewDescriptor.planeCount = 1;
		}

		for (uint32 ImageIndex = 0; ImageIndex < BackBufferImages.Size(); ++ImageIndex)
		{
			TextureViews[ImageIndex] = static_cast<VulkanTextureView*>(pDevice->CreateTextureView(&TextureViewDescriptor, BackBufferImages[ImageIndex]));
		}
	}

	void VulkanViewport::DestroySwapchain()
	{
	}

	void VulkanViewport::RecreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
	}

}