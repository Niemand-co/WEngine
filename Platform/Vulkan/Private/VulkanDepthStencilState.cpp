#include "pch.h"
#include "Platform/Vulkan/Public/VulkanDepthStencilState.h"

namespace Vulkan
{

	VulkanDepthStencilState::VulkanDepthStencilState(const RHIDepthStencilStateInitializer& Initializer)
	{
		DepthStencilStateCreateInfo = {};
		DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		DepthStencilStateCreateInfo.depthTestEnable = Initializer.bEnableDepthTest;
		DepthStencilStateCreateInfo.depthWriteEnable = Initializer.bEnableDepthWrite;
		DepthStencilStateCreateInfo.depthCompareOp = WEngine::ToVulkan(Initializer.DepthCompareOp);
		DepthStencilStateCreateInfo.depthBoundsTestEnable = false;
		DepthStencilStateCreateInfo.stencilTestEnable = Initializer.bEnableFrontStencil || Initializer.bEnableBackStencil;
		if (Initializer.bEnableFrontStencil)
		{
			DepthStencilStateCreateInfo.front.compareOp = WEngine::ToVulkan(Initializer.FrontStencilCompareOp);
			DepthStencilStateCreateInfo.front.failOp = WEngine::ToVulkan(Initializer.FrontStencilFailedOp);
			DepthStencilStateCreateInfo.front.depthFailOp = WEngine::ToVulkan(Initializer.FrontDepthFailedOp);
			DepthStencilStateCreateInfo.front.passOp = WEngine::ToVulkan(Initializer.FrontStencilPassOp);
			DepthStencilStateCreateInfo.front.writeMask = Initializer.StencilWriteMask;
			DepthStencilStateCreateInfo.front.compareMask = Initializer.StencilReadMask;
			DepthStencilStateCreateInfo.front.reference = 1;
		}
		if (Initializer.bEnableBackStencil)
		{
			DepthStencilStateCreateInfo.back.compareOp = WEngine::ToVulkan(Initializer.BackStencilCompareOp);
			DepthStencilStateCreateInfo.back.failOp = WEngine::ToVulkan(Initializer.BackStencilFailedOp);
			DepthStencilStateCreateInfo.back.depthFailOp = WEngine::ToVulkan(Initializer.BackDepthFailedOp);
			DepthStencilStateCreateInfo.back.passOp = WEngine::ToVulkan(Initializer.BackStencilPassOp);
			DepthStencilStateCreateInfo.back.writeMask = Initializer.StencilWriteMask;
			DepthStencilStateCreateInfo.back.compareMask = Initializer.StencilReadMask;
			DepthStencilStateCreateInfo.back.reference = 1;
		}
	}

}