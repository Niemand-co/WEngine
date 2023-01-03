#pragma once
#include "RHI/Public/RHIViewport.h"

namespace Vulkan
{

	class VulkanViewport : public RHIViewport
	{
	public:

		VulkanViewport(VkViewport *pViewport);

		virtual ~VulkanViewport();

		const VkViewport* GetHandle() const { return m_pViewport; }

	private:

		VkViewport *m_pViewport;

	};

}