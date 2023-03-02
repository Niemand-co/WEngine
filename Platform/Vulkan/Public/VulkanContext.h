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

		virtual void RHIBeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor) override;

		virtual void RHIEndRenderPass() override;

		virtual void RHIBeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions) override;

		virtual void CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY) override;

	private:

		VulkanCommandBufferManager *pCommandBufferManager;

	};

}