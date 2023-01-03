#pragma once
#include "RHI/Public/RHIScissor.h"

namespace Vulkan
{

	class VulkanScissor : public RHIScissor
	{
	public:

		VulkanScissor(VkRect2D *pScissor);

		virtual ~VulkanScissor();

		const VkRect2D* GetHandle() const { return m_pScissor; }

	private:

		VkRect2D *m_pScissor;

	};

}