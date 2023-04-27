#pragma once
#include "RHI/Public/RHIContext.h"

namespace Vulkan
{

	class VulkanStaticContext : public StaticRHIContext
	{
	public:

		VulkanStaticContext() = default;

		virtual ~VulkanStaticContext() = default;

		virtual WAttachmentBlendStateRHIRef CreateBlendState(const RHIBlendStateInitializer& Initializer) override;

		virtual WDepthStencilStateRHIRef CreateDepthStencilState(const RHIDepthStencilStateInitializer& Initializer) override;

		virtual WRasterizationStateRHIRef CreateRasterizationState(const RHIRasterizationStateInitializer& Initializer) override;

		virtual WMultiSampleStateRHIRef CreateMultiSampleState(const RHIMultiSampleStateInitializer& Initializer) override;

		virtual WVertexInputStateRHIRef GetOrCreateVertexInputState(const WEngine::WArray<VertexInputElement>& InElements) override;

	};

	class VulkanDynamicContext : public DynamicRHIContext
	{
	public:

		VulkanDynamicContext();

		virtual ~VulkanDynamicContext();

		class VulkanCommandBufferManager* GetCmdBufferManager() { return pCommandBufferManager; }

		virtual void RHIBeginDrawingViewport(RHIViewport* Viewport) override;

		virtual void RHIEndDrawingViewport(RHIViewport* Viewport, bool bPresent) override;

		virtual WRenderPassRHIRef RHIBeginRenderPass(RHIRenderPassDescriptor* RenderPasDescriptor, RHIFramebufferDescriptor* FramebufferDescriptor) override;

		virtual void RHIEndRenderPass() override;

		virtual void RHISetViewport(float MinX, float MinY, float MaxX, float MaxY, float MinZ, float MaxZ) override;

		virtual void RHISetScissor(uint32 MinX, uint32 MinY, uint32 Width, uint32 Height) override;

		virtual void RHIBindVertexBuffer(WVertexFactory* InVertexFactory) override;

		virtual void RHISetStreamResource(uint32 StreamIndex, WVertexBufferRHIRef Buffer, uint32 Offset) override;

		virtual void RHIBindIndexBuffer(WIndexBufferRHIRef InIndexBuffer) override;

		virtual void RHIDrawIndexedPrimitive(uint32 indexCount, uint32 firstIndex, uint32 instanceCount) override;

		virtual void RHIBeginTransition(WEngine::WArray<RHIBarrierDescriptor>& Transitions) override;

		virtual void RHICopyImageToBackBuffer(RHITexture* SrcTexture, RHITexture* DstTexture, int32 SrcSizeX, int32 SrcSizeY, int32 DstSizeX, int32 DstSizeY) override;

		virtual void RHISetGraphicsPipelineState(RHIGraphicsPipelineState *GraphicsPipelineState) override;

		virtual void RHISetShaderUniformBuffer(RHIGraphicsShader* ShaderRHI, uint32 BufferIndex, WUniformBufferRHIRef UniformBuffer) override;

		virtual void RHIUpdateUniformBuffer(WUniformBufferRHIRef UniformBuffer, void* Contents) override;

		virtual RHIPipelineStateObject* RHICreateGraphicsPipelineState(RHIGraphicsPipelineStateInitializer& Initializer) override;

		virtual RHIRenderPass* RHICreateRenderPass(const RHIGraphicsPipelineStateInitializer& Initializer) override;

		virtual WSamplerStateRHIRef RHICreateSamplerState(const RHISamplerStateInitializer& Initializer) override;

	private:

		VulkanCommandBufferManager *pCommandBufferManager;

		class VulkanPendingGfxState *PendingState;

		class VulkanPendingComputeState *PendingComputeState;

	};

}