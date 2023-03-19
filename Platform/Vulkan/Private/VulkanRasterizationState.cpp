#include "pch.h"
#include "Platform/Vulkan/Public/VulkanRasterizationState.h"

namespace Vulkan
{

	VulkanRasterizationState::VulkanRasterizationState(const RHIRasterizationStateInitializer& Initializer)
	{
		RasterizationStateCreateInfo = {};
		RasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		RasterizationStateCreateInfo.cullMode = WEngine::ToVulkan(Initializer.CullMode);
		RasterizationStateCreateInfo.depthClampEnable = Initializer.bEnableDepthClamp;
		RasterizationStateCreateInfo.rasterizerDiscardEnable = Initializer.bEnableDiscard;
		RasterizationStateCreateInfo.polygonMode = WEngine::ToVulkan(Initializer.PolygonMode);
		RasterizationStateCreateInfo.frontFace = Initializer.bClockWise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
		RasterizationStateCreateInfo.lineWidth = 1.0f;
		RasterizationStateCreateInfo.depthBiasEnable = false;

		InputAssemblyStateCreateInfo = {};
		InputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		InputAssemblyStateCreateInfo.primitiveRestartEnable = false;
		InputAssemblyStateCreateInfo.topology = WEngine::ToVulkan(Initializer.PrimitiveTopology);
	}

}