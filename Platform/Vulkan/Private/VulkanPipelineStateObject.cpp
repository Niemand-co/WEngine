#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPipelineStateObject.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

namespace Vulkan
{

	WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*> VulkanPipelineStateManager::GraphicsPipelines = WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*>();

	WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*> VulkanPipelineStateManager::ComputePipelines = WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*>();

	VulkanGraphicsPipelineStateObject::VulkanGraphicsPipelineStateObject(VulkanDevice *pInDevice, VkGraphicsPipelineCreateInfo *pInfo)
		: pDevice(pInDevice)
	{
		vkCreateGraphicsPipelines(pDevice->GetHandle(), VK_NULL_HANDLE, 1, pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Pipeline);
	}

	VulkanGraphicsPipelineStateObject::~VulkanGraphicsPipelineStateObject()
	{
		vkDestroyPipeline(pDevice->GetHandle(), Pipeline, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanGraphicsPipelineStateObject::Bind(RHICommandBuffer* CmdBuffer)
	{
		vkCmdBindPipeline(static_cast<VulkanCommandBuffer*>(CmdBuffer)->GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	}

}