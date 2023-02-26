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
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

namespace Vulkan
{

	VulkanViewport::VulkanViewport(VulkanDevice *pInDevice, RHIViewportDescriptor* descriptor)
		: pDevice(pInDevice),
		  PixelFormat(descriptor->format),
		  Width(descriptor->Width),
		  Height(descriptor->Height)
	{
		CreateSwapchain();
		for (uint32 index = 0; index < RenderingDoneSemaphores.Size(); ++index)
		{
			RenderingDoneSemaphores[index] = new VulkanSemaphore(pInDevice);
		}
	}

	VulkanViewport::~VulkanViewport()
	{
		if(!pAcquireImageSemaphore)
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
			bFailedToAcquireImage = false;
		}

		CmdBuffer->EndScopePass();
		VulkanCommandBufferManager *ImmediateCmdBufferMgr = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager();
		if (!bFailedToAcquireImage)
		{
			//TrueCmdBuffer->AddWaitingSemaphore(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, pAcquireImageSemaphore);
			//ImmediateCmdBufferMgr->SubmitActiveCommandBufferFromPresent(RenderingDoneSemaphores[AcquiredImageIndex]);
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

		EState PresentState = (EState)pSwapchain->Present(Queue, pAcquireImageSemaphore);
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

		LastFrameCmdBuffer = static_cast<VulkanQueue*>(RHIContext::GetContext()->GetQueue())->GetLastSubmittedCmdBuffer();
	}

	void VulkanViewport::CreateSwapchain()
	{
		RHISwapchainDescriptor SwapchainDescriptor = {};
		{
			SwapchainDescriptor.instance = RHIContext::GetContext()->GetInstance();
			SwapchainDescriptor.count = 3;
			SwapchainDescriptor.extent = Extent(Width, Height);
			SwapchainDescriptor.colorSpace = ColorSpace::SRGB_Linear;
			SwapchainDescriptor.format = PixelFormat;
		}
		pSwapchain = static_cast<VulkanSwapchain*>(pDevice->CreateSwapchain(&SwapchainDescriptor, BackBufferImages));

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

	void VulkanViewport::RecreateSwapchain()
	{
		WEngine::WScopeLock ScopeLock(&SwapchainRecreationSection);
		pSwapchain->Recreate();
	}

}