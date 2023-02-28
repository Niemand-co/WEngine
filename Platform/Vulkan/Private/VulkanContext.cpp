#include "pch.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"

namespace Vulkan
{

	VulkanContext::VulkanContext()
	{
		pCommandBufferManager = new VulkanCommandBufferManager(static_cast<VulkanDevice*>(pDevice), static_cast<VulkanQueue*>(pQueue));
	}

	VulkanContext::~VulkanContext()
	{
	}

	void VulkanContext::RHIBeginDrawingViewport(RHIViewport* Viewport)
	{
		VulkanViewport *TrueViewport = dynamic_cast<VulkanViewport*>(Viewport);
		RE_ASSERT(TrueViewport != nullptr, "Error RHI.");
	}

	void VulkanContext::RHIEndDrawingViewport(RHIViewport* Viewport, bool bPresent)
	{
		if (bPresent)
		{
			VulkanCommandBuffer *ActiveCmdBuffer = pCommandBufferManager->GetActiveCommandBuffer();
			bool bSuccefullyPresent = Viewport->Present(ActiveCmdBuffer, pQueue);
		}
	}

	void VulkanContext::RHIBeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions)
	{
		VulkanPipelineBarrier PipelineBarrier;
		for (RHIBarrierDescriptor& Transition : Transitions)
		{

			VkPipelineStageFlags SrcPipelineStage, DstPipelineStage;
			VkAccessFlags SrcAccess, DstAccess;
			VkImageLayout SrcLayout, DstLayout;
			GetVkStageAndAccessFlags(Transition.AccessBefore, false, SrcPipelineStage, SrcAccess, SrcLayout);
			GetVkStageAndAccessFlags(Transition.AccessAfter, false, DstPipelineStage, DstAccess, DstLayout);

			if (Transition.Type == RHIBarrierDescriptor::EType::Texture)
			{
				VulkanTextureBase *TextureBase = VulkanTextureBase::Cast(Transition.Texture);
				VkImageSubresourceRange Range = {};
				{
					Range.aspectMask = TextureBase->GetSurface().GetImageAspect();
					Range.baseArrayLayer = Transition.ArrayLayer;
					Range.layerCount = 1;
					Range.baseMipLevel = Transition.MipLevel;
					Range.levelCount = 1;
				}

				if(SrcLayout == DstLayout)
					PipelineBarrier.AddTransition(TextureBase->GetHandle(), Range, SrcAccess, DstAccess, SrcLayout);
				else
					PipelineBarrier.AddTransition(TextureBase->GetHandle(), Range, SrcAccess, DstAccess, SrcLayout, DstLayout);
			}
			else if (Transition.Type == RHIBarrierDescriptor::EType::Buffer)
			{
				VulkanBufferBase *BufferBase = VulkanBufferBase::Cast(Transition.Buffer);
				PipelineBarrier.AddTransition(BufferBase->GetHandle(), Transition.Range, Transition.Offset, SrcAccess, DstAccess);
			}
		}
		PipelineBarrier.Execute(static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->GetImmediateCommandBuffer());
	}

}