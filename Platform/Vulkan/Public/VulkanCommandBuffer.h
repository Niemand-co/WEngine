#pragma once
#include "RHI/Public/RHICommandBuffer.h"

namespace Vulkan
{

	class VulkanCommandBuffer : public RHICommandBuffer
	{
	public:

		VulkanCommandBuffer(VkCommandBuffer *commandBuffer,VkCommandPool *pCommandPool, bool isSecondary, VkDevice *pDevice);

		virtual ~VulkanCommandBuffer();

		virtual void BeginScopePass(std::string passName, RHIRenderPass* pRenderPass = nullptr, unsigned int subpass = 0, RHIRenderTarget* pRenderTarget = nullptr) override;

		virtual void EndScopePass() override;

		virtual void ExecuteCommandBuffer(RHICommandBuffer* pCommandBuffer) override;

		virtual RHIGraphicsEncoder* GetGraphicsEncoder() override;

		virtual RHIComputeEncoder* GetComputeEncoder() override;

		virtual void Clear() override;

		virtual VkCommandBuffer* GetHandle();

	private:

		VkCommandBuffer *m_commandBuffer;

		VkCommandPool *m_pCommandPool;

		VkDevice *m_pDevice;

		bool m_isSecondary;

	};

}