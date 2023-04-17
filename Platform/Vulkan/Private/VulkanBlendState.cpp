#include "pch.h"
#include "Platform/Vulkan/Public/VulkanBlendState.h"

namespace Vulkan
{

	VulkanBlendState::VulkanBlendState(const RHIBlendStateInitializer& Initializer)
	{
		ColorBlendAttachmentState = {};
		ColorBlendAttachmentState.blendEnable = Initializer.bEnableBlend;
		ColorBlendAttachmentState.colorBlendOp = WEngine::ToVulkan(Initializer.ColorBlendOp);
		ColorBlendAttachmentState.srcColorBlendFactor = WEngine::ToVulkan(Initializer.ColorSrcFactor);
		ColorBlendAttachmentState.srcColorBlendFactor = WEngine::ToVulkan(Initializer.ColorSrcFactor);
		ColorBlendAttachmentState.alphaBlendOp = WEngine::ToVulkan(Initializer.AlphaBlendOp);
		ColorBlendAttachmentState.srcAlphaBlendFactor = WEngine::ToVulkan(Initializer.AlphaSrcFactor);
		ColorBlendAttachmentState.srcAlphaBlendFactor = WEngine::ToVulkan(Initializer.AlphaSrcFactor);
		ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	}

}