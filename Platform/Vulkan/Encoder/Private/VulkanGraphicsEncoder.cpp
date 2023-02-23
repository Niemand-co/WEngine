#include "pch.h"
#include "Platform/Vulkan/Encoder/Public/VulkanGraphicsEncoder.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"
#include "RHI/Public/RHIScissor.h"
#include "RHI/Public/RHIViewport.h"
#include "Utils/Public/Window.h"
#include "Editor/Public/Screen.h"

namespace Vulkan
{

	VulkanGraphicsEncoder::VulkanGraphicsEncoder(VkCommandBuffer *cmd)
		: m_cmd(cmd)
	{
		m_width = 0;
		m_height = 0;
	}

	VulkanGraphicsEncoder::~VulkanGraphicsEncoder()
	{
	}



}