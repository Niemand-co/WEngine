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

		virtual WRenderPassRHIRef RHIBeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor) override;

		virtual void RHIEndRenderPass() override;

		virtual void RHISetViewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth) override;

		virtual void RHISetScissor() override;

		virtual void RHIDrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount) override;

		virtual void RHIBeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions) override;

		virtual void CopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY) override;

		virtual void RHISetGraphicsPipelineState(class RHIGraphicsPipelineStateDescriptor* descriptor) override;

		virtual WBlendStateRHIRef CreateBlendState(const RHIBlendStateInitializer& Initializer) override;

		virtual WDepthStencilStateRHIRef CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer) override;

		virtual WRasterizationStateRHIRef CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer) override;

		virtual WMultiSampleStateRHIRef CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer) override;

		virtual WVertexInputStateRHIRef CreateVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements) override;

	private:

		VulkanCommandBufferManager *pCommandBufferManager;

	};

}