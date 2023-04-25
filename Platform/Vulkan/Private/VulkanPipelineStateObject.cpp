#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"

namespace Vulkan
{

	VulkanGraphicsPipelineStateObject::VulkanGraphicsPipelineStateObject(VulkanDevice *pInDevice, const GfxPipelineDesc& InDesc)
		: pDevice(pInDevice),
		  Desc(InDesc)
	{
	}

	VulkanGraphicsPipelineStateObject::~VulkanGraphicsPipelineStateObject()
	{
	}

	void VulkanGraphicsPipelineStateObject::Bind(RHICommandBuffer* CmdBuffer)
	{
		vkCmdBindPipeline(static_cast<VulkanCommandBuffer*>(CmdBuffer)->GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
	}

	VulkanGraphicsPipelineStateObject* VulkanPipelineStateManager::RHICreateGraphicsPipelineState(const RHIGraphicsPipelineStateInitializer& Initializer)
	{
		GfxPipelineDesc Desc;
		VulkanDescriptorSetLayout DescriptorSetLayout;
		CreateGfxEntry(Initializer, DescriptorSetLayout, Desc);

		VulkanGraphicsPipelineStateObject *NewPSO = new VulkanGraphicsPipelineStateObject(pDevice, Desc);
		if (!CreateGfxPipelineFromtEntry(Initializer, NewPSO))
		{
			
			return nullptr;
		}

		return NewPSO;
	}

	void VulkanPipelineStateManager::CreateGfxEntry(const RHIGraphicsPipelineStateInitializer& Initializer, VulkanDescriptorSetLayout& DescriptorSetLayout, GfxPipelineDesc& Desc)
	{
		DescriptorSetLayout.ProcessBindingForStage(VK_SHADER_STAGE_VERTEX_BIT);

		if (Initializer.BoundShaderState.PixelShaderRHI)
		{
			DescriptorSetLayout.ProcessBindingForStage(VK_SHADER_STAGE_FRAGMENT_BIT);
		}

		if (Initializer.BoundShaderState.GeometryShaderRHI)
		{
			DescriptorSetLayout.ProcessBindingForStage(VK_SHADER_STAGE_GEOMETRY_BIT);
		}
		
		Desc.SubpassIndex = Initializer.SubpassIndex;

		Desc.ColorBlendAttachmentStates.AddInitialized(Initializer.RenderTargetEnabled);
		for (int32 Index = 0; Index < Initializer.RenderTargetEnabled; ++Index)
		{
			VulkanBlendState *BlendState = ResourceCast(Initializer.BlendState);
			Desc.ColorBlendAttachmentStates[Index].ReadFrom(BlendState->Attachments[Index]->ColorBlendAttachmentState);
		}

		Desc.DepthStencil.ReadFrom(ResourceCast(Initializer.DepthStencilState)->DepthStencilStateCreateInfo);

		Desc.Rasterizer.ReadFrom(ResourceCast(Initializer.RasterizationState)->RasterizationStateCreateInfo);

		VulkanVertexInputState *VertexInputState = ResourceCast(Initializer.BoundShaderState.VertexInputState);
		Desc.VertexBindings.AddInitialized(VertexInputState->Bindings.Size());
		for (int32 Index = 0; Index < Desc.VertexBindings.Size(); ++Index)
		{
			Desc.VertexBindings[Index].ReadFrom(VertexInputState->Bindings[Index]);
		}
		Desc.VertexAttributes.AddInitialized(VertexInputState->NumAttributes);
		for (int32 Index = 0; Index < Desc.VertexAttributes.Size(); ++Index)
		{
			Desc.VertexAttributes[Index].ReadFrom(VertexInputState->VertexInputAttributes[Index]);
		}

	}
	bool VulkanPipelineStateManager::CreateGfxPipelineFromtEntry(const RHIGraphicsPipelineStateInitializer& Initializer, VulkanGraphicsPipelineStateObject* PSO)
	{
		const GfxPipelineDesc& Desc = PSO->Desc;

		VkGraphicsPipelineCreateInfo GfxPipelineCreateInfo;
		ZeroVulkanStruct(GfxPipelineCreateInfo, VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);

		VkPipelineColorBlendStateCreateInfo BlendStateInfo;
		ZeroVulkanStruct(BlendStateInfo, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
		VkPipelineColorBlendAttachmentState Attachments[MaxSimultaneousRenderTargets];
		BlendStateInfo.attachmentCount = Desc.ColorBlendAttachmentStates.Size();
		BlendStateInfo.pAttachments = Attachments;
		for (int32 Index = 0; Index < Desc.ColorBlendAttachmentStates.Size(); ++Index)
		{
			Desc.ColorBlendAttachmentStates[Index].WriteInto(Attachments[Index]);
		}
		GfxPipelineCreateInfo.pColorBlendState = &BlendStateInfo;

		VkPipelineDepthStencilStateCreateInfo DepthStencilStateInfo;
		ZeroVulkanStruct(DepthStencilStateInfo, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
		Desc.DepthStencil.WriteInto(DepthStencilStateInfo);
		GfxPipelineCreateInfo.pDepthStencilState = &DepthStencilStateInfo;

		VkPipelineRasterizationStateCreateInfo RasterizationStateInfo;
		ZeroVulkanStruct(RasterizationStateInfo, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
		Desc.Rasterizer.WriteInto(RasterizationStateInfo);
		GfxPipelineCreateInfo.pRasterizationState = &RasterizationStateInfo;

		VkPipelineMultisampleStateCreateInfo MultiSampleStateInfo;
		ZeroVulkanStruct(MultiSampleStateInfo, VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
		GfxPipelineCreateInfo.pMultisampleState = &MultiSampleStateInfo;
		
		VkPipelineVertexInputStateCreateInfo VertexInputStateInfo;
		ZeroVulkanStruct(VertexInputStateInfo, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
		VertexInputStateInfo.vertexBindingDescriptionCount = Desc.VertexBindings.Size();
		VkVertexInputBindingDescription VertexBindings[MaxVertexInputElementCount];
		VertexInputStateInfo.pVertexBindingDescriptions = VertexBindings;
		for (int32 Index = 0; Index < Desc.VertexBindings.Size(); ++Index)
		{
			Desc.VertexBindings[Index].WriteInto(VertexBindings[Index]);
		}
		VkVertexInputAttributeDescription VertexAttributes[MaxVertexInputElementCount];
		VertexInputStateInfo.pVertexAttributeDescriptions = VertexAttributes;
		for (int32 Index = 0; Index < Desc.VertexAttributes.Size(); ++Index)
		{
			Desc.VertexAttributes[Index].WriteInto(VertexAttributes[Index]);
		}
		GfxPipelineCreateInfo.pVertexInputState = &VertexInputStateInfo;

		VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo;
		ZeroVulkanStruct(InputAssemblyStateInfo, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
		InputAssemblyStateInfo.topology = (VkPrimitiveTopology	)Desc.Topology;
		GfxPipelineCreateInfo.pInputAssemblyState = &InputAssemblyStateInfo;

		VkPipelineViewportStateCreateInfo ViewportStateInfo;
		ZeroVulkanStruct(ViewportStateInfo, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
		ViewportStateInfo.viewportCount = 1;
		ViewportStateInfo.scissorCount = 1;
		GfxPipelineCreateInfo.pViewportState = &ViewportStateInfo;

		VkShaderModule ShaderModules[(uint8)EShaderStage::Count];
		const char* ShaderEntries[(uint8)EShaderStage::Count];
		GetShaderModule(Initializer, ShaderModules, ShaderEntries);
		VkPipelineShaderStageCreateInfo ShaderStageInfo[(uint8)EShaderStage::Count];
		GfxPipelineCreateInfo.pStages = ShaderStageInfo;
		for (int32 Index = 0; Index < (int32)EShaderStage::Count; ++Index)
		{
			ZeroVulkanStruct(ShaderStageInfo[GfxPipelineCreateInfo.stageCount], VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
			ShaderStageInfo[GfxPipelineCreateInfo.stageCount].stage = WEngine::ToVulkan((EShaderStage)Index);
			ShaderStageInfo[GfxPipelineCreateInfo.stageCount].module = ShaderModules[Index];
			ShaderStageInfo[GfxPipelineCreateInfo.stageCount].pName = ShaderEntries[Index];
			GfxPipelineCreateInfo.stageCount++;
		}

		VkPipelineDynamicStateCreateInfo DynamicStateInfo;
		ZeroVulkanStruct(DynamicStateInfo, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
		VkDynamicState DynamicStates[5];
		DynamicStateInfo.dynamicStateCount = 5;
		DynamicStateInfo.pDynamicStates = DynamicStates;
		DynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
		DynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;
		DynamicStates[2] = VK_DYNAMIC_STATE_LINE_WIDTH;
		DynamicStates[3] = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		DynamicStates[4] = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		GfxPipelineCreateInfo.pDynamicState = &DynamicStateInfo;

		GfxPipelineCreateInfo.renderPass = PSO->RenderPass->GetHandle();
		GfxPipelineCreateInfo.layout = PSO->Layout;
		GfxPipelineCreateInfo.subpass = Desc.SubpassIndex;

		VkResult Result = vkCreateGraphicsPipelines(pDevice->GetHandle(), VK_NULL_HANDLE, 1, &GfxPipelineCreateInfo, ResourceCast(NormalAllocator::Get())->GetCallbacks(), &PSO->Pipeline);
		if (Result != VK_SUCCESS)
		{
			RE_LOG("Failed to create pipeline state object.");
			return false;
		}

		return true;
	}

	void VulkanPipelineStateManager::GetShaderModule(const RHIGraphicsPipelineStateInitializer& Initializer, VkShaderModule OutShaderModules[(uint8)EShaderStage::Count], const char* OutShaderEntries[(uint8)EShaderStage::Count])
	{
		if (Initializer.BoundShaderState.VertexShaderRHI)
		{
			OutShaderModules[(uint8)EShaderStage::Vertex] = ResourceCast(Initializer.BoundShaderState.VertexShaderRHI)->GetShaderModule();
			OutShaderEntries[(uint8)EShaderStage::Vertex] = "VSMain";
		}

		if (Initializer.BoundShaderState.PixelShaderRHI)
		{
			OutShaderModules[(uint8)EShaderStage::Pixel] = ResourceCast(Initializer.BoundShaderState.PixelShaderRHI)->GetShaderModule();
			OutShaderEntries[(uint8)EShaderStage::Pixel] = "PSMain";
		}

		if (Initializer.BoundShaderState.GeometryShaderRHI)
		{
			OutShaderModules[(uint8)EShaderStage::Geometry] = ResourceCast(Initializer.BoundShaderState.GeometryShaderRHI)->GetShaderModule();
			OutShaderEntries[(uint8)EShaderStage::Geometry] = "GSMain";
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