#include "pch.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanCommandPool.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanView.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.H"

namespace Vulkan
{

	static void CopyImageToBackBuffer(VulkanCommandBuffer *CmdBuffer, VkImage SrcImage, VkImage DstImage, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY)
	{
		VkImageLayout SrcLayout = VulkanTextureLayoutManager::FindLayout(SrcImage);
		VkImageLayout DstLayout = VulkanTextureLayoutManager::FindLayout(DstImage);

		{
			VulkanPipelineBarrier Barrier;
			VkImageSubresourceRange Range = VulkanPipelineBarrier::GetTextureSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
			if (SrcLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(SrcImage, Range, SrcLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			if (DstLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
				Barrier.AddTransition(DstImage, Range, DstLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			Barrier.Execute(CmdBuffer);
		}

		if (SrcSizeX != DstSizeX || SrcSizeY != DstSizeY)
		{
			VkImageBlit Region = {};
			{
				Region.srcOffsets[0].x = 0;
				Region.srcOffsets[0].y = 0;
				Region.srcOffsets[0].z = 0;
				Region.srcOffsets[1].x = SrcSizeX;
				Region.srcOffsets[1].y = SrcSizeY;
				Region.srcOffsets[1].z = 1;
				Region.dstOffsets[0].x = 0;
				Region.dstOffsets[0].y = 0;
				Region.dstOffsets[0].z = 0;
				Region.dstOffsets[1].x = DstSizeX;
				Region.dstOffsets[1].y = DstSizeY;
				Region.dstOffsets[1].z = 1;
				Region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.srcSubresource.layerCount = 1;
				Region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.dstSubresource.layerCount = 1;
			}

			vkCmdBlitImage(CmdBuffer->GetHandle(), SrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region, VK_FILTER_LINEAR);
		}
		else
		{
			VkImageCopy Region = {};
			{
				Region.extent.width = SrcSizeX;
				Region.extent.height = SrcSizeY;
				Region.extent.depth = 1;
				Region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.srcSubresource.layerCount = 1;
				Region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				Region.dstSubresource.layerCount = 1;
			}

			vkCmdCopyImage(CmdBuffer->GetHandle(), SrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, DstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);
		}

		{
			VulkanPipelineBarrier Barrier;
			VkImageSubresourceRange Range = VulkanPipelineBarrier::GetTextureSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
			if (SrcLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
				Barrier.AddTransition(SrcImage, Range, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, SrcLayout);
			if (DstLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
				Barrier.AddTransition(DstImage, Range, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, DstLayout);
			Barrier.Execute(CmdBuffer);
		}
	}

	VulkanViewport::VulkanViewport(VulkanDevice *pInDevice, RHIViewportDescriptor* descriptor)
		: pDevice(pInDevice),
		  PixelFormat(descriptor->format),
		  Width(descriptor->Width),
		  Height(descriptor->Height),
		  pAcquireImageSemaphore(nullptr)
	{
		CreateSwapchain();
		//for (uint32 index = 0; index < RenderingDoneSemaphores.Size(); ++index)
		//{
		//	RenderingDoneSemaphores[index] = new VulkanSemaphore(pInDevice);
		//}
	}

	VulkanViewport::~VulkanViewport()
	{
		if(pAcquireImageSemaphore)
			delete pAcquireImageSemaphore;
		delete pSwapchain;
	}

	void VulkanViewport::Tick()
	{
	}

	bool VulkanViewport::AcquireImageIndex()
	{
		int32 NewImageIndex = pSwapchain->AcquireImageIndex(&pAcquireImageSemaphore);
		if (NewImageIndex != -1)
		{
			AcquiredImageIndex = NewImageIndex;
			return true;
		}
		else
		{
			return false;
		}

	}

	int32 VulkanViewport::Present(RHICommandBuffer* CmdBuffer, RHIQueue* Queue)
	{
		VulkanCommandBuffer *TrueCmdBuffer = dynamic_cast<VulkanCommandBuffer*>(CmdBuffer);
		RE_ASSERT(TrueCmdBuffer && TrueCmdBuffer->IsOutsideRenderPass(), "");

		bool bFailedToAcquireImage = true;
		if (AcquireImageIndex())
		{
			CopyImageToBackBuffer(TrueCmdBuffer, RenderingBackBuffer->GetHandle(), BackBufferImages[AcquiredImageIndex], Width, Height, pSwapchain->Width, pSwapchain->Height);
			bFailedToAcquireImage = false;
		}

		CmdBuffer->EndScopePass();
		VulkanCommandBufferManager *ImmediateCmdBufferMgr = static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager();
		if (!bFailedToAcquireImage)
		{
			TrueCmdBuffer->AddWaitingSemaphore(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, pAcquireImageSemaphore);
			ImmediateCmdBufferMgr->SubmitActiveCommandBufferFromPresent(RenderingDoneSemaphores[AcquiredImageIndex]);
		}
		else
		{
			static_cast<VulkanQueue*>(Queue)->Submit(TrueCmdBuffer);
			RecreateSwapchain();

			pDevice->SubmitCommandsAndFlushGPU();
			pDevice->Wait();

			return (int32)EState::Healthy;
		}

		if(AcquiredImageIndex == -1)
			return (int32)EState::Healthy;

		EState PresentState = (EState)pSwapchain->Present(Queue, RenderingDoneSemaphores[AcquiredImageIndex]);
		int32 AttemptsPending = 4;
		while ((int32)PresentState < 0 && AttemptsPending > 0)
		{
			if (PresentState == EState::OutofData)
			{
				RE_LOG("Swapchain is out of data! Trying to recreate the swapchain.");
			}
			else if (PresentState == EState::SurfaceLost)
			{
				RE_LOG("Surface is lost! Trying to reacreate the swapchain and surface.");
			}

			RecreateSwapchain();

			pDevice->SubmitCommandsAndFlushGPU();
			pDevice->Wait();

			--AttemptsPending;
		}

		bool bResult = (int32)PresentState >= 0;
		if (!bResult)
		{
			RE_LOG("Failed to present image.");
		}

		WaitForFrameEvenCompletion();

		AcquiredImageIndex = -1;

		if (ImmediateCmdBufferMgr->GetActiveCommandBuffer() && !ImmediateCmdBufferMgr->GetActiveCommandBuffer()->HasBegun())
		{
			ImmediateCmdBufferMgr->PrepareForNewActiveCmdBuffer();
		}

		return bResult;
	}

	void VulkanViewport::WaitForFrameEvenCompletion()
	{
		static WCriticalSection section;
		WEngine::WScopeLock Lock(&section);

		if (LastFrameCmdBuffer && LastFrameCmdBuffer->IsSubmitted())
		{
			if (!LastFrameCmdBuffer->GetFence()->IsFenceSignaled())
			{
				LastFrameCmdBuffer->GetOwner()->GetManager()->WaitForCommandBuffer(LastFrameCmdBuffer);
			}
		}

		LastFrameCmdBuffer = static_cast<VulkanQueue*>(GetDynamicRHI()->GetQueue())->GetLastSubmittedCmdBuffer();
	}

	WTexture2DRHIRef VulkanViewport::GetRenderTarget() const
	{
		return RenderingBackBuffer.Get();
	}

	void VulkanViewport::CreateSwapchain()
	{
		RHISwapchainDescriptor SwapchainDescriptor = {};
		{
			SwapchainDescriptor.instance = GetDynamicRHI()->GetInstance();
			SwapchainDescriptor.count = 3;
			SwapchainDescriptor.extent = Extent(Width, Height);
			SwapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
			SwapchainDescriptor.format = PixelFormat;
		}
		pSwapchain = static_cast<VulkanSwapchain*>(pDevice->CreateSwapchain(&SwapchainDescriptor, BackBufferImages));

		for (VkImage Image : BackBufferImages)
		{
			VulkanTextureLayoutManager::AddLayout(Image, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		}

		RenderingDoneSemaphores.Resize(BackBufferImages.Size());
		TextureViews.Resize(BackBufferImages.Size());

		RHITextureViewDescriptor TextureViewDescriptor = {};
		{
			TextureViewDescriptor.Format = PixelFormat;
			TextureViewDescriptor.Dimension = EDimension::Texture2D;
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

		RenderingBackBuffer = static_cast<VulkanTexture2D*>(GetRenderCommandList()->CreateTexture2D(Width, Height, PixelFormat, 1, { 1, 0, 0, 1 }, ETextureCreateFlags::TextureCreate_RenderTarget | ETextureCreateFlags::TextureCreate_SRV, EAccess::RTV));
	}

	void VulkanViewport::DestroySwapchain()
	{
	}

	void VulkanViewport::RecreateSwapchain()
	{
		WEngine::WScopeLock ScopeLock(&SwapchainRecreationSection);
		pSwapchain->Recreate();
	}

}