#pragma once
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"

namespace Vulkan
{

	class VulkanGraphicsEncoder : public RHIGraphicsEncoder
	{
	public:

		VulkanGraphicsEncoder(VkCommandBuffer *cmd);

		virtual ~VulkanGraphicsEncoder();



	private:

		VkCommandBuffer *m_cmd;

	};

}