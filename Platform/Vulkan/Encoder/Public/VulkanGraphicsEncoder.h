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

		virtual void SetViewport(RHIViewport *viewports) override;

		virtual void SetScissor(RHIScissor* scissor) override;

		virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth /* = 0.0f */) override;

		virtual void BindVertexBuffer(RHIBuffer *pBuffer) override;

		virtual void BindIndexBuffer(RHIBuffer *pBuffer) override;

		virtual void BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout *pPipelineResourceLayout) override;

		virtual void DrawVertexArray() override;

		virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) override;

		virtual void NextSubpass() override;

		virtual void EndPass() override;

	private:

		VkCommandBuffer *m_cmd;

	};

}