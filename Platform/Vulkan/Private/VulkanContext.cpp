#include "pch.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"
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

	void VulkanContext::RHIBeginTransition(RHIBarrierBatch* Barrier)
	{
		VulkanPipelineBarrier PipelineBarrier;
		for (auto& BarrierPair : Barrier->GetBarrierBatches())
		{
			RHIResource *resource = BarrierPair.First();
			RHIBarrierDescriptor& descriptor = BarrierPair.Second();
			
			if (descriptor.Type == RHIBarrierDescriptor::EType::Texture)
			{
				PipelineBarrier.AddTransition(descriptor.Texture, )
			}
		}
	}

}