#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPipelineStateObject.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanDescriptorSet.h"

namespace Vulkan
{

	WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*> VulkanPipelineStateManager::GraphicsPipelines = WEngine::WHashMap<uint32, VulkanGraphicsPipelineStateObject*>();

	WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*> VulkanPipelineStateManager::ComputePipelines = WEngine::WHashMap<uint32, VulkanComputePipelineStateObject*>();

	VulkanGraphicsPipelineStateObject::VulkanGraphicsPipelineStateObject(VulkanDevice *pInDevice, VkGraphicsPipelineCreateInfo *pInfo)
		: pDevice(pInDevice)
	{
		RE_ASSERT(vkCreateGraphicsPipelines(pDevice->GetHandle(), VK_NULL_HANDLE, 1, pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Pipeline) == VK_SUCCESS, "Failed to create pipeline");
	}

	VulkanGraphicsPipelineStateObject::~VulkanGraphicsPipelineStateObject()
	{
		vkDestroyPipeline(pDevice->GetHandle(), Pipeline, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks());
	}

	void VulkanGraphicsPipelineStateObject::Bind(RHICommandBuffer* CmdBuffer)
	{
		vkCmdBindPipeline(static_cast<VulkanCommandBuffer*>(CmdBuffer)->GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	}

	VulkanGraphicsPipelineDescriptorState* VulkanPipelineStateManager::RHICreateGraphicsPipelineState(const RHIGraphicsPipelineStateInitializer& Initializer)
	{
		GfxPipelineDesc Desc;
		VulkanDescriptorSetLayout DescriptorSetLayout;
		CreateGfxEntry(Initializer, DescriptorSetLayout, Desc);
	}

	void VulkanPipelineStateManager::CreateGfxEntry(const RHIGraphicsPipelineStateInitializer& Initializer, VulkanDescriptorSetLayout& DescriptorSetLayout, GfxPipelineDesc& Desc)
	{
		Desc.ColorBlendAttachmentStates.AddInitialized(Initializer.RenderTargetEnabled);
		for (int32 Index = 0; Index < Initializer.RenderTargetEnabled; ++Index)
		{

		}
	}

	void GfxPipelineDesc::FBlendAttachment::ReadFrom(const VkPipelineColorBlendAttachmentState& InState)
	{
		bBlend              = InState.blendEnable == VK_TRUE;
		ColorBlendOp        = (uint8)InState.colorBlendOp;
		ColorBlendSrcFactor = (uint8)InState.srcColorBlendFactor;
		ColorBlendDstFactor = (uint8)InState.dstColorBlendFactor;
		AlphaBlendOp        = (uint8)InState.alphaBlendOp;
		AlphaBlendSrcFactor = (uint8)InState.srcAlphaBlendFactor;
		AlphaBlendDstFactor = (uint8)InState.dstAlphaBlendFactor;
		ColorWriteMask      = (uint8)InState.colorWriteMask;
	}

	void GfxPipelineDesc::FBlendAttachment::WriteInto(VkPipelineColorBlendAttachmentState& OutState) const
	{
		OutState.blendEnable         = bBlend ? VK_TRUE : VK_FALSE;
		OutState.colorBlendOp		 = (VkBlendOp)ColorBlendOp;
		OutState.srcColorBlendFactor = (VkBlendFactor)ColorBlendSrcFactor;
		OutState.dstColorBlendFactor = (VkBlendFactor)ColorBlendDstFactor;
		OutState.alphaBlendOp		 = (VkBlendOp)AlphaBlendOp;
		OutState.srcAlphaBlendFactor = (VkBlendFactor)AlphaBlendSrcFactor;
		OutState.dstAlphaBlendFactor = (VkBlendFactor)AlphaBlendDstFactor;
		OutState.colorWriteMask		 = (VkColorComponentFlags)ColorWriteMask;
	}

	void GfxPipelineDesc::FVertexBinding::ReadFrom(const VkVertexInputBindingDescription& InState)
	{
		Binding = InState.binding;
		Stride = InState.stride;
		InputRate = (uint16)InState.inputRate;
	}

	void GfxPipelineDesc::FVertexBinding::WriteInto(VkVertexInputBindingDescription& OutState) const
	{
		OutState.binding = Binding;
		OutState.stride = Stride;
		OutState.inputRate = (VkVertexInputRate)InputRate;
	}

	void GfxPipelineDesc::FVertexAttribute::ReadFrom(const VkVertexInputAttributeDescription& InState)
	{
		Location = InState.location;
		Binding = InState.binding;
		Offset = InState.offset;
		Format = (uint32)InState.format;
	}

	void GfxPipelineDesc::FVertexAttribute::WriteInto(VkVertexInputAttributeDescription& OutState) const
	{
		OutState.location = Location;
		OutState.binding = Binding;
		OutState.offset = Offset;
		OutState.format = (VkFormat)Format;
	}

	void GfxPipelineDesc::FRasterizer::ReadFrom(const VkPipelineRasterizationStateCreateInfo& InState)
	{
		PolygonMode = InState.polygonMode;
		CullMode = InState.cullMode;
		DepthBiasSlopeScale = InState.depthBiasSlopeFactor;
		DepthBiasConstantFactor = InState.depthBiasConstantFactor;
	}

	void GfxPipelineDesc::FRasterizer::WriteInto(VkPipelineRasterizationStateCreateInfo& OutState) const
	{
		OutState.polygonMode = (VkPolygonMode)PolygonMode;
		OutState.cullMode = (VkCullModeFlags)CullMode;
		OutState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		OutState.depthClampEnable = VK_FALSE;
		OutState.depthBiasEnable = DepthBiasConstantFactor != 0.0f ? VK_TRUE : VK_FALSE;
		OutState.rasterizerDiscardEnable = VK_FALSE;
		OutState.depthBiasSlopeFactor = DepthBiasSlopeScale;
		OutState.depthBiasConstantFactor = DepthBiasConstantFactor;
	}

	void GfxPipelineDesc::FDepthStencil::ReadFrom(const VkPipelineDepthStencilStateCreateInfo& InState)
	{
		DepthCompareOp = (uint8)InState.depthCompareOp;
		bDepthTestEnable = InState.depthTestEnable != VK_FALSE;
		bDepthWriteEnable = InState.depthWriteEnable != VK_FALSE;
		bDepthBoundsTestEnable = InState.depthBoundsTestEnable != VK_FALSE;
		bStencilTestEnable = InState.stencilTestEnable != VK_FALSE;
		FrontFailOp = (uint8)InState.front.failOp;
		FrontPassOp = (uint8)InState.front.passOp;
		FrontDepthFailOp = (uint8)InState.front.depthFailOp;
		FrontCompareOp = (uint8)InState.front.compareOp;
		FrontCompareMask = (uint8)InState.front.compareMask;
		FrontWriteMask = InState.front.writeMask;
		FrontReference = InState.front.reference;
		BackFailOp = (uint8)InState.back.failOp;
		BackPassOp = (uint8)InState.back.passOp;
		BackDepthFailOp = (uint8)InState.back.depthFailOp;
		BackCompareOp = (uint8)InState.back.compareOp;
		BackCompareMask = (uint8)InState.back.compareMask;
		BackWriteMask = InState.back.writeMask;
		BackReference = InState.back.reference;
	}

	void GfxPipelineDesc::FDepthStencil::WriteInto(VkPipelineDepthStencilStateCreateInfo& OutState) const
	{
		OutState.depthCompareOp = (VkCompareOp)DepthCompareOp;
		OutState.depthTestEnable = bDepthTestEnable;
		OutState.depthWriteEnable = bDepthWriteEnable;
		OutState.depthBoundsTestEnable = bDepthBoundsTestEnable;
		OutState.stencilTestEnable = bStencilTestEnable;
		OutState.front.failOp = (VkStencilOp)FrontFailOp;
		OutState.front.passOp = (VkStencilOp)FrontPassOp;
		OutState.front.depthFailOp = (VkStencilOp)FrontDepthFailOp;
		OutState.front.compareOp = (VkCompareOp)FrontCompareOp;
		OutState.front.compareMask = FrontCompareMask;
		OutState.front.writeMask = FrontWriteMask;
		OutState.front.reference = FrontReference;
		OutState.back.failOp = (VkStencilOp)BackFailOp;
		OutState.back.passOp = (VkStencilOp)BackPassOp;
		OutState.back.depthFailOp = (VkStencilOp)BackDepthFailOp;
		OutState.back.compareOp = (VkCompareOp)BackCompareOp;
		OutState.back.writeMask = BackWriteMask;
		OutState.back.compareMask = BackCompareMask;
		OutState.back.reference = BackReference;
	}

}