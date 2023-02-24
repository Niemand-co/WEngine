#pragma once
#include "RHI/Public/RHIContext.h"

namespace Vulkan
{

	class VulkanContext : public RHIContext
	{
	public:

		VulkanContext();

		virtual ~VulkanContext();

		class VulkanCommandBufferManager* GetCmdBufferManager() { return pCommandBufferManager; }

		virtual void RHIBeginDrawingViewport(class RHIViewport* Viewport) override;

		virtual void RHIEndDrawingViewport(class RHIViewport* Viewport, bool bPresent) override;

	private:

		VulkanCommandBufferManager *pCommandBufferManager;

	};

}