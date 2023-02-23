#include "pch.h"
#include "Platform/Vulkan/Public/VulkanContext.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanViewport.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Encoder/Public/VulkanComputeEncoder.h"

namespace Vulkan
{

	VulkanContext::VulkanContext()
	{
		pCommandBufferManager = new VulkanCommandBufferManager(static_cast<VulkanDevice*>(pDevice));
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
			Viewport->Present();
		}
	}

}