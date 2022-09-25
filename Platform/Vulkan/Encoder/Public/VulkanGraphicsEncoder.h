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

		virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, Vector4 clearColor, float clearDepth /* = 0.0f */) override;

		virtual void EndPass() override;

	private:

		VkCommandBuffer *m_cmd;

	};

}