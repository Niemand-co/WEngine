#pragma once
#include "RHI/Encoder/Public/RHIComputeEncoder.h"

namespace Vulkan
{

	class VulkanComputeEncoder : public RHIComputeEncoder
	{
	public:

		VulkanComputeEncoder();

		virtual ~VulkanComputeEncoder();

	};

}