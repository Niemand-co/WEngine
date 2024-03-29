#pragma once
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"

namespace Vulkan
{

	class VulkanGraphicsEncoder : public RHIGraphicsEncoder
	{
	public:

		VulkanGraphicsEncoder(VkCommandBuffer *cmd);

		virtual ~VulkanGraphicsEncoder();

		virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) override;

		virtual void SetPipeline(RHIPipelineStateObject *pso) override;

		virtual void SetViewport(const RHIViewport& viewports) override;

		virtual void SetScissor(const RHIScissor& scissor) override;

		virtual void SetLineWidth(float width) override;

		virtual void SetDepthTestEnable(bool enable) override;

		virtual void SetPolygonTopology(PrimitiveTopology primitive) override;

		virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth /* = 0.0f */, unsigned int clearStencil /* = 1 */) override;

		virtual void BindVertexBuffer(RHIBuffer *pBuffer) override;

		virtual void BindIndexBuffer(RHIBuffer *pBuffer) override;

		virtual void BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout *pPipelineResourceLayout, unsigned int offsetCount, unsigned int* offsets) override;

		virtual void DrawVertexArray() override;

		virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) override;

		virtual void ResourceBarrier(unsigned int srcStage, unsigned int dstStage) override;

		virtual void ResourceBarrier(RHIBarrierDescriptor* pDescriptor) override;

		virtual void CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height) override;

		virtual void SetEvent(RHIEvent* pEvent) override;

		virtual void NextSubpass() override;

		virtual void EndPass() override;

	private:

		VkCommandBuffer *m_cmd;

	};

}