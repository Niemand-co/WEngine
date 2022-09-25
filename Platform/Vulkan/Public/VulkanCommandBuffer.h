#pragma once
#include "RHI/Public/RHICommandBuffer.h"

namespace Vulkan
{

	class VulkanCommandBuffer : public RHICommandBuffer
	{
	public:

		VulkanCommandBuffer(VkCommandBuffer *commandBuffer);

		virtual ~VulkanCommandBuffer();

		virtual void BeginScopePass(std::string passName) override;

		virtual void EndScopePass() override;

		virtual RHIGraphicsEncoder* GetGraphicsEncoder() override;

		virtual RHIComputeEncoder* GetComputeEncoder() override;

		virtual VkCommandBuffer* GetHandle();

	private:

		VkCommandBuffer *m_commandBuffer;

	};

}