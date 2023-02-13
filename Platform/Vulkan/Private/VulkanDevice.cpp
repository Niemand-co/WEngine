#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Vertex.h"
#include "RHI/Public/RHIBuffer.h"
#include "RHI/Public/RHIContext.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VulkanGPU *pInGPU, VkDeviceCreateInfo* pInfo)
		: pGPU(pInGPU)
	{
		RE_ASSERT(vkCreateDevice(*pInGPU->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &pDevice) == VK_SUCCESS, "Failed to Create Device.");
	}

	VulkanDevice::~VulkanDevice()
	{

	}

	unsigned int VulkanDevice::GetQueueCount(RHIQueueType type)
	{
		for(unsigned int i = 0; i < m_queues.Size(); ++i)
			if(m_queues[i].type == type)
				return m_queues[i].count;
		return 0;
	}

	RHIQueue* VulkanDevice::GetQueue(RHIQueueType type, unsigned int count)
	{
		unsigned int queueFamilyID = 0;
		for (; queueFamilyID < m_queues.Size(); ++queueFamilyID)
			if (((uint8)m_queues[queueFamilyID].type & (uint8)type) > 0)
				break;

		return new VulkanQueue(this, queueFamilyID);
	}

	RHISwapchain* VulkanDevice::CreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = *static_cast<VulkanInstance*>(descriptor->instance)->GetSurface();
		swapchainCreateInfo.imageFormat = WEngine::ToVulkan(descriptor->format);
		swapchainCreateInfo.imageColorSpace = WEngine::ToVulkan(descriptor->colorSpace);
		swapchainCreateInfo.presentMode = WEngine::ToVulkan(descriptor->presenMode);
		swapchainCreateInfo.imageExtent = { descriptor->extent.width, descriptor->extent.height };
		swapchainCreateInfo.minImageCount = descriptor->count;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;

		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		return new VulkanSwapchain(this, static_cast<VulkanInstance*>(RHIContext::GetInstance()), &swapchainCreateInfo);
	}

	WEngine::WArray<RHIFence*> VulkanDevice::CreateFence(unsigned int count)
	{
		WEngine::WArray<RHIFence*> fences(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences[i] = new VulkanFence(this);
		}

		return fences;
	}

	RHIEvent* VulkanDevice::GetEvent()
	{
		VkEventCreateInfo eventCreateInfo = {};
		{
			eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		}

		VkEvent* pEvent = (VkEvent*)NormalAllocator::Get()->Allocate(sizeof(VkEvent));
		vkCreateEvent(*m_pDevice, &eventCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pEvent);
		
		RHIEvent *event = (RHIEvent*)NormalAllocator::Get()->Allocate(sizeof(VulkanEvent));
		::new (event) VulkanEvent(pEvent);

		return event;
	}

	WVertexShaderRHIRef VulkanDevice::CreateVertexShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule* pShaderModule = (VkShaderModule*)NormalAllocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_pDevice, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pShaderModule);

		return new VulkanVertexShader(pShaderModule);
	}

	WPixelShaderRHIRef VulkanDevice::CreatePixelShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule* pShaderModule = (VkShaderModule*)NormalAllocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_pDevice, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pShaderModule);

		return new VulkanPixelShader(pShaderModule);
	}

	WGeometryShaderRHIRef VulkanDevice::CreateGeometryShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule* pShaderModule = (VkShaderModule*)NormalAllocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_pDevice, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pShaderModule);

		return new VulkanGeometryShader(pShaderModule);
	}

	WComputeShaderRHIRef VulkanDevice::CreateComputeShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule* pShaderModule = (VkShaderModule*)NormalAllocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_pDevice, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pShaderModule);

		return new VulkanComputeShader(pShaderModule);
	}

	RHIRenderPass* VulkanDevice::CreateRenderPass(RHIRenderPassDescriptor* descriptor)
	{
		VkAttachmentDescription *pAttachmentDescriptions = (VkAttachmentDescription*)NormalAllocator::Get()->Allocate(descriptor->attachmentCount * sizeof(VkAttachmentDescription));
		for (unsigned int i = 0; i < descriptor->AttachmentCount; ++i)
		{
			RHIAttachmentDescriptor *pAttachmentDescriptor = descriptor->AttachmentDescriptors + i;
			::new (pAttachmentDescriptions + i) VkAttachmentDescription();
			pAttachmentDescriptions[i].format = WEngine::ToVulkan(pAttachmentDescriptor->attachmentFormat);
			pAttachmentDescriptions[i].samples = WEngine::ToVulkan(pAttachmentDescriptor->sampleCount);
			pAttachmentDescriptions[i].loadOp = WEngine::ToVulkan(pAttachmentDescriptor->attachmentLoadOP);
			pAttachmentDescriptions[i].storeOp = WEngine::ToVulkan(pAttachmentDescriptor->attachmentStoreOP);
			pAttachmentDescriptions[i].stencilLoadOp = WEngine::ToVulkan(pAttachmentDescriptor->stencilLoadOP);
			pAttachmentDescriptions[i].stencilStoreOp = WEngine::ToVulkan(pAttachmentDescriptor->stencilStoreOP);
			pAttachmentDescriptions[i].initialLayout = WEngine::ToVulkan(pAttachmentDescriptor->initialLayout);
		}

		WEngine::WArray<VkSubpassDescription> pSubpassDescriptions(descriptor->SubpassCount);
		WEngine::WArray<VkAttachmentReference*> pColorAttachmentReferences(descriptor->SubpassCount);
		WEngine::WArray<VkAttachmentReference*> pDepthAttachmentReferences(descriptor->SubpassCount, nullptr);
		WEngine::WArray<VkAttachmentReference*> pInputAttachmentReferences(descriptor->SubpassCount);

		for (unsigned int i = 0; i < descriptor->SubpassCount; ++i)
		{
			RHISubPassDescriptor& SubpassDescriptor = descriptor->SubPassDescriptors[i];

			pColorAttachmentReferences[i] = (VkAttachmentReference*)NormalAllocator::Get()->Allocate(SubpassDescriptor.ColorAttachmentCount * sizeof(VkAttachmentReference));
			for (unsigned int j = 0; j < SubpassDescriptor.ColorAttachmentCount; ++j)
			{
				::new (pColorAttachmentReferences[i] + j) VkAttachmentReference();
				pColorAttachmentReferences[i][j].attachment = (SubpassDescriptor.ColorAttachments[j]).attachmentIndex;
				pColorAttachmentReferences[i][j].layout = WEngine::ToVulkan((SubpassDescriptor.ColorAttachments[j]).attachmentLayout);
				pAttachmentDescriptions[(SubpassDescriptor.ColorAttachments[j]).attachmentIndex].finalLayout = pColorAttachmentReferences[i][j].layout;
			}

			if (SubpassDescriptor.bHasDepthAttachment)
			{
				pDepthAttachmentReferences[i] = (VkAttachmentReference*)NormalAllocator::Get()->Allocate(sizeof(VkAttachmentReference));
				::new (pDepthAttachmentReferences[i]) VkAttachmentReference();
				pDepthAttachmentReferences[i]->attachment = SubpassDescriptor.DepthStencilAttachment.attachmentIndex;
				pDepthAttachmentReferences[i]->layout = WEngine::ToVulkan(SubpassDescriptor.DepthStencilAttachment.attachmentLayout);
				pAttachmentDescriptions[SubpassDescriptor.DepthStencilAttachment.attachmentIndex].finalLayout = pDepthAttachmentReferences[i]->layout;
			}

			pInputAttachmentReferences[i] = (VkAttachmentReference*)NormalAllocator::Get()->Allocate(SubpassDescriptor.InputAttachmentCount * sizeof(VkAttachmentReference));
			for (unsigned int j = 0; j < SubpassDescriptor.InputAttachmentCount; ++j)
			{
				::new (pInputAttachmentReferences[i] + j) VkAttachmentReference();
				pInputAttachmentReferences[i][j].attachment = (SubpassDescriptor.InputAttachments[j]).attachmentIndex;
				pInputAttachmentReferences[i][j].layout = WEngine::ToVulkan((SubpassDescriptor.InputAttachments[j]).attachmentLayout);
				pAttachmentDescriptions[(SubpassDescriptor.InputAttachments[j]).attachmentIndex].finalLayout = pInputAttachmentReferences[i][j].layout;
			}

			pSubpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			pSubpassDescriptions[i].colorAttachmentCount = SubpassDescriptor.ColorAttachmentCount;
			pSubpassDescriptions[i].pColorAttachments = pColorAttachmentReferences[i];
			pSubpassDescriptions[i].pDepthStencilAttachment = pDepthAttachmentReferences[i];
			pSubpassDescriptions[i].inputAttachmentCount = SubpassDescriptor.InputAttachmentCount;
			pSubpassDescriptions[i].pInputAttachments = pInputAttachmentReferences[i];


		}

		WEngine::WArray<VkSubpassDependency> pSubpassDependencies(descriptor->DependencyCount);
		for (unsigned int i = 0; i < descriptor->DependencyCount; ++i)
		{
			RHISubPassDependencyDescriptor *dependencyDescriptor = descriptor->DependencyDescriptors + i;
			pSubpassDependencies[i].srcSubpass = dependencyDescriptor->dependedPass >= 0 ? dependencyDescriptor->dependedPass : VK_SUBPASS_EXTERNAL;
			pSubpassDependencies[i].dstSubpass = dependencyDescriptor->waitingPass >= 0 ? dependencyDescriptor->waitingPass : VK_SUBPASS_EXTERNAL;
			pSubpassDependencies[i].srcStageMask = dependencyDescriptor->dependedStage;
			pSubpassDependencies[i].srcAccessMask = dependencyDescriptor->dependedAccess;
			pSubpassDependencies[i].dstStageMask = dependencyDescriptor->waitingStage;
			pSubpassDependencies[i].dstAccessMask = dependencyDescriptor->waitingAccess;
		}

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = descriptor->AttachmentCount;
		renderPassCreateInfo.pAttachments = pAttachmentDescriptions;
		renderPassCreateInfo.subpassCount = descriptor->SubpassCount;
		renderPassCreateInfo.pSubpasses = pSubpassDescriptions.GetData();
		renderPassCreateInfo.dependencyCount = descriptor->DependencyCount;
		renderPassCreateInfo.pDependencies = pSubpassDependencies.GetData();

		VkRenderPass *pRenderPass = (VkRenderPass*)NormalAllocator::Get()->Allocate(sizeof(VkRenderPass));
   		RE_ASSERT(vkCreateRenderPass(pDevice, &renderPassCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pRenderPass) == VK_SUCCESS, "Failed to Create Render Pass.");

		RHIRenderPass *renderPass = (RHIRenderPass*)NormalAllocator::Get()->Allocate(sizeof(VulkanRenderPass));
		::new (renderPass) VulkanRenderPass(pRenderPass);

		return renderPass;
	}

	RHIPipelineStateObject* VulkanDevice::CreatePipelineStateObject(RHIPipelineStateObjectDescriptor* descriptor)
	{

		//WEngine::WArray<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(descriptor->shaderCount);
		//for (unsigned int i = 0; i < descriptor->shaderCount; ++i)
		//{
		//	RHIShader* shader = descriptor->pShader[i];
		//	shaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		//	shaderStageCreateInfos[i].stage = (VkShaderStageFlagBits)shader->GetStage();
		//	shaderStageCreateInfos[i].module = *static_cast<VulkanShaderBase*>(shader)->GetShaderModule();
		//	shaderStageCreateInfos[i].pName = shader->GetEntry();
		//}

		VkVertexInputBindingDescription vertexInputBindgDescription = {};
		{
			vertexInputBindgDescription.binding = descriptor->vertexDescriptor->bindingDescription->slot;
			vertexInputBindgDescription.stride = descriptor->vertexDescriptor->bindingDescription->stride;
			vertexInputBindgDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}
		VkVertexInputAttributeDescription *pVertexInputAttributeDescriptions = (VkVertexInputAttributeDescription*)NormalAllocator::Get()->Allocate(descriptor->vertexDescriptor->attributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
		for(unsigned int i = 0; i < descriptor->vertexDescriptor->attributeDescriptionCount; ++i)
		{
			pVertexInputAttributeDescriptions[i].binding = descriptor->vertexDescriptor->pAttributeDescription[i]->slot;
			pVertexInputAttributeDescriptions[i].location = descriptor->vertexDescriptor->pAttributeDescription[i]->location;
			pVertexInputAttributeDescriptions[i].offset = descriptor->vertexDescriptor->pAttributeDescription[i]->offset;
			pVertexInputAttributeDescriptions[i].format = WEngine::ToVulkan(descriptor->vertexDescriptor->pAttributeDescription[i]->format);
		}
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		{
			vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
			vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindgDescription;
			vertexInputStateCreateInfo.vertexAttributeDescriptionCount = descriptor->vertexDescriptor->attributeDescriptionCount;
			vertexInputStateCreateInfo.pVertexAttributeDescriptions = pVertexInputAttributeDescriptions;
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
		{
			inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyStateCreateInfo.topology = WEngine::ToVulkan(descriptor->rasterizationStateDescriptor->primitivePology);
			inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
		}

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		{
			viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateCreateInfo.scissorCount = descriptor->scissorCount;
			viewportStateCreateInfo.pScissors = ((VulkanScissor*)descriptor->pScissors)->GetHandle();
			viewportStateCreateInfo.viewportCount = descriptor->viewportCount;
			viewportStateCreateInfo.pViewports = ((VulkanViewport*)descriptor->pViewports)->GetHandle();
		}

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
		{
			rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			rasterizationStateCreateInfo.polygonMode = WEngine::ToVulkan(descriptor->rasterizationStateDescriptor->polygonMode);
			rasterizationStateCreateInfo.lineWidth = descriptor->rasterizationStateDescriptor->lineWidth;
			rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
			rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
			rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
			rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
		}

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
		{
			multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
			multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleStateCreateInfo.minSampleShading = 1.0f;
			multisampleStateCreateInfo.pSampleMask = nullptr;
			multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		}

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
		{
			colorBlendAttachmentState.blendEnable = descriptor->blendDescriptor->blendEnabled;
			colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachmentState.srcColorBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->colorSrcFactor);
			colorBlendAttachmentState.dstColorBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->colorDstFactor);
			colorBlendAttachmentState.colorBlendOp = WEngine::ToVulkan(descriptor->blendDescriptor->colorBlendOP);
			colorBlendAttachmentState.srcAlphaBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->alphaSrcFactor);
			colorBlendAttachmentState.dstAlphaBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->alphaDstFactor);
			colorBlendAttachmentState.alphaBlendOp = WEngine::ToVulkan(descriptor->blendDescriptor->alphaBlendOP);
		}

		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
		{
			depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilStateCreateInfo.stencilTestEnable = descriptor->depthStencilDescriptor->stencilTestEnabled;
			depthStencilStateCreateInfo.back.failOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->stencilFailedOP);
			depthStencilStateCreateInfo.back.compareMask = 0xFF;
			depthStencilStateCreateInfo.back.writeMask = 0xFF;
			depthStencilStateCreateInfo.back.passOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->passOP);
			depthStencilStateCreateInfo.back.depthFailOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->depthFailedOP);
			depthStencilStateCreateInfo.back.compareOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->stencilCompareOP);
			depthStencilStateCreateInfo.back.reference = descriptor->depthStencilDescriptor->stencilRef;
			depthStencilStateCreateInfo.front = depthStencilStateCreateInfo.back;
			depthStencilStateCreateInfo.depthTestEnable = descriptor->depthStencilDescriptor->depthTestEnabled;
			depthStencilStateCreateInfo.depthCompareOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->depthCompareOP);
			depthStencilStateCreateInfo.depthWriteEnable = descriptor->depthStencilDescriptor->depthWriteEnabled;
			depthStencilStateCreateInfo.depthBoundsTestEnable = descriptor->depthStencilDescriptor->depthBoundsTest;
			depthStencilStateCreateInfo.maxDepthBounds = descriptor->depthStencilDescriptor->maxDepth;
			depthStencilStateCreateInfo.minDepthBounds = descriptor->depthStencilDescriptor->minDepth;
		}

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
		{
			colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendStateCreateInfo.attachmentCount = 1;
			colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
			colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
			colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
			colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
			colorBlendStateCreateInfo.blendConstants[3] = 0.0f;
		}

		VkDynamicState dynamicStates[3] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		{
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.dynamicStateCount = 3;
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
		}

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
		{
			graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			//graphicsPipelineCreateInfo.stageCount = shaderStageCreateInfos.Size();
			graphicsPipelineCreateInfo.stageCount = 0;
			//graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.GetData();
			graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
			graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
			graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
			graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
			graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
			graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
			graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
			graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			graphicsPipelineCreateInfo.layout = *static_cast<VulkanPipelineResourceLayout*>(descriptor->pipelineResourceLayout)->GetHandle();
			graphicsPipelineCreateInfo.renderPass = *static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
			graphicsPipelineCreateInfo.subpass = descriptor->subpass;
			graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			//graphicsPipelineCreateInfo.basePipelineIndex = -1;
		}

		VkPipeline *pPipeline = (VkPipeline*)NormalAllocator::Get()->Allocate(sizeof(VkPipeline));
		RE_ASSERT(vkCreateGraphicsPipelines(pDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pPipeline) == VK_SUCCESS, "Failed to Create Pipeline.");

		for (unsigned int i = 0; i < descriptor->vertexDescriptor->attributeDescriptionCount; ++i)
		{
			pVertexInputAttributeDescriptions[i].~VkVertexInputAttributeDescription();
		}
		NormalAllocator::Get()->Deallocate(pVertexInputAttributeDescriptions);

		RHIPipelineStateObject *pipeline = (RHIPipelineStateObject*)NormalAllocator::Get()->Allocate(sizeof(VulkanAllocator));
		::new (pipeline) VulkanPipelineStateObject(pPipeline);

		return pipeline;
	}

	WTexture2DRHIRef VulkanDevice::CreateTexture2D(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, 1 };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.usage = descriptor->usage;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

		return new VulkanTexture2D(this, &imageCreateInfo, descriptor->aspect);
	}

	WTexture2DArrayRHIRef VulkanDevice::CreateTexture2DArray(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, 1 };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.usage = descriptor->usage;
		imageCreateInfo.arrayLayers = descriptor->layerCount;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

		return new VulkanTexture2DArray(this, &imageCreateInfo, descriptor->aspect);
	}

	WTexture3DRHIRef VulkanDevice::CreateTexture3D(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, descriptor->depth };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_3D;
		imageCreateInfo.usage = descriptor->usage;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

		return new VulkanTexture3D(this, &imageCreateInfo, descriptor->aspect);
	}

	RHISampler* VulkanDevice::CreateSampler(RHISamplerDescriptor* descriptor)
	{
		VkSamplerCreateInfo samplerCreateInfo = {};
		{
			samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerCreateInfo.anisotropyEnable = false;
			samplerCreateInfo.maxAnisotropy = 16;
			samplerCreateInfo.magFilter = WEngine::ToVulkan(descriptor->magFilter);
			samplerCreateInfo.minFilter = WEngine::ToVulkan(descriptor->minFilter);
			samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerCreateInfo.compareEnable = false;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerCreateInfo.minLod = 0;
			samplerCreateInfo.maxLod = 0;
			samplerCreateInfo.mipLodBias = 0;
		}

		VkSampler *pSampler = (VkSampler*)NormalAllocator::Get()->Allocate(sizeof(VkSampler));
		RE_ASSERT(vkCreateSampler(pDevice, &samplerCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pSampler) == VK_SUCCESS, "Failed to Create Sampler.");
		
		VulkanSampler *sampler = (VulkanSampler*)NormalAllocator::Get()->Allocate(sizeof(VulkanSampler));
		::new (sampler) VulkanSampler(pSampler);

		return sampler;
	}

	RHIRenderTarget* VulkanDevice::CreateRenderTarget(RHIRenderTargetDescriptor* descriptor)
	{
		WEngine::WArray<VkImageView> views(descriptor->bufferCount);
		for (unsigned int i = 0; i < views.Size(); ++i)
		{
			views[i] = *(static_cast<VulkanTextureView*>(descriptor->pBufferView[i])->GetHandle());
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.attachmentCount = views.Size();
		framebufferCreateInfo.pAttachments = views.GetData();
		framebufferCreateInfo.renderPass = *static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
		framebufferCreateInfo.width = descriptor->width;
		framebufferCreateInfo.height = descriptor->height;
		framebufferCreateInfo.layers = 1;

		VkFramebuffer *framebuffer = (VkFramebuffer*)NormalAllocator::Get()->Allocate(sizeof(VkFramebuffer));
		RE_ASSERT(vkCreateFramebuffer(pDevice, &framebufferCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), framebuffer) == VK_SUCCESS, "Failed to Create Framebuffer.");

		VulkanRenderTarget *renderTarget = (VulkanRenderTarget*)NormalAllocator::Get()->Allocate(sizeof(VulkanRenderTarget));
		::new (renderTarget) VulkanRenderTarget(framebuffer, descriptor->width, descriptor->height, &pDevice);

		return renderTarget;
	}

	WVertexBufferRHIRef VulkanDevice::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->stride * descriptor->count;
		VkBufferCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			info.size = bufferSize;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		
		return new VulkanVertexBuffer(this, &info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	WDynamicVertexBufferRHIRef VulkanDevice::CreateDynamicVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->stride * descriptor->count;
		size_t minUBOSize = pGPU->GetFeature().minUBOAlignment;
		bufferSize = (bufferSize + minUBOSize - 1) & ~(minUBOSize - 1);
		VkBufferCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			info.size = bufferSize;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		return new VulkanDynamicVertexBuffer(this, &info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}

	WIndexBufferRHIRef VulkanDevice::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->stride * descriptor->count;
		VkBufferCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			info.size = bufferSize;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		return new VulkanIndexBuffer(this, &info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	WUniformBufferRHIRef VulkanDevice::CreateUniformBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->stride * descriptor->count;
		VkBufferCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			info.size = bufferSize;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		return new VulkanUniformBuffer(this, &info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	WDynamicUniformBufferRHIRef VulkanDevice::CreateDynamicUniformBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->stride * descriptor->count;
		size_t minUBOSize = pGPU->GetFeature().minUBOAlignment;
		bufferSize = (bufferSize + minUBOSize - 1) & ~(minUBOSize - 1);
		VkBufferCreateInfo info = {};
		{
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			info.size = bufferSize;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		return new VulkanDynamicUniformBuffer(this, &info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}

	RHIGroup* VulkanDevice::CreateResourceGroup(RHIGroupDescriptor* descriptor)
	{

		return nullptr;
	}

	RHIGroupLayout* VulkanDevice::CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor)
	{
		VkDescriptorSetLayoutBinding *pDescriptorSetLayoutBindings = (VkDescriptorSetLayoutBinding*)NormalAllocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkDescriptorSetLayoutBinding));
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			::new (pDescriptorSetLayoutBindings + i) VkDescriptorSetLayoutBinding();
			BindingResource *resource = (descriptor->pBindingResources) + i;
			pDescriptorSetLayoutBindings[i].binding = resource->bindingSlot;
			pDescriptorSetLayoutBindings[i].descriptorCount = resource->count;
			pDescriptorSetLayoutBindings[i].descriptorType = WEngine::ToVulkan(resource->type);
			pDescriptorSetLayoutBindings[i].stageFlags = resource->shaderStage;
			pDescriptorSetLayoutBindings[i].pImmutableSamplers = nullptr;
		}
		
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		{
			descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutCreateInfo.bindingCount = descriptor->bindingCount;
			descriptorSetLayoutCreateInfo.pBindings = pDescriptorSetLayoutBindings;
		}

		VkDescriptorSetLayout *pDescriptorSetLayout = (VkDescriptorSetLayout*)NormalAllocator::Get()->Allocate(sizeof(VkDescriptorSetLayout));
		::new (pDescriptorSetLayout) VkDescriptorSetLayout();
		RE_ASSERT(vkCreateDescriptorSetLayout(pDevice, &descriptorSetLayoutCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pDescriptorSetLayout) == VK_SUCCESS, "Failed to Create Descriptor Set Layout.");

		VulkanGroupLayout *groupLayout = (VulkanGroupLayout*)NormalAllocator::Get()->Allocate(sizeof(VulkanGroupLayout));
		::new (groupLayout) VulkanGroupLayout(pDescriptorSetLayout, descriptor->bindingCount);

		groupLayout->bindingCount = descriptor->bindingCount;
		groupLayout->pBindingResources = descriptor->pBindingResources;
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			BindingResource* resource = (descriptor->pBindingResources) + i;
			pDescriptorSetLayoutBindings[i].~VkDescriptorSetLayoutBinding();
		}
		NormalAllocator::Get()->Deallocate(pDescriptorSetLayoutBindings);

		return groupLayout;
	}

	RHIPipelineResourceLayout* VulkanDevice::CreatePipelineResourceLayout(RHIPipelineResourceLayoutDescriptor* descriptor)
	{
		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		{
			layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			layoutCreateInfo.setLayoutCount = descriptor->groupLayoutCount;
			layoutCreateInfo.pSetLayouts = static_cast<VulkanGroupLayout*>(descriptor->pGroupLayout)->GetHandle();
			layoutCreateInfo.pushConstantRangeCount = 0;
			layoutCreateInfo.pPushConstantRanges = nullptr;
		}
		VkPipelineLayout* pLayout = (VkPipelineLayout*)NormalAllocator::Get()->Allocate(sizeof(VkPipelineLayout));
		RE_ASSERT(vkCreatePipelineLayout(pDevice, &layoutCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pLayout) == VK_SUCCESS, "Failed to Create Pipeline Layout.");

		RHIPipelineResourceLayout *pPipelineResourceLayout = (RHIPipelineResourceLayout*)NormalAllocator::Get()->Allocate(sizeof(VulkanPipelineResourceLayout));
		::new (pPipelineResourceLayout) VulkanPipelineResourceLayout(pLayout);

		return pPipelineResourceLayout;
	}

	RHIGroupPool* VulkanDevice::CreateGroupPool(RHIGroupPoolDescriptor* descriptor)
	{
		VkDescriptorPoolSize *pDescriptorPoolSizes = (VkDescriptorPoolSize*)NormalAllocator::Get()->Allocate(descriptor->pGroupLayout->bindingCount * sizeof(VkDescriptorPoolSize));
		for (unsigned int i = 0; i < descriptor->pGroupLayout->bindingCount; ++i)
		{
			::new (pDescriptorPoolSizes + i) VkDescriptorPoolSize();
			pDescriptorPoolSizes[i].descriptorCount = descriptor->pGroupLayout->pBindingResources[i].count;
			pDescriptorPoolSizes[i].type = WEngine::ToVulkan(descriptor->pGroupLayout->pBindingResources[i].type);
		}

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
		{
			descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			descriptorPoolCreateInfo.maxSets = descriptor->maxSetCount;
			descriptorPoolCreateInfo.poolSizeCount = descriptor->pGroupLayout->bindingCount;
			descriptorPoolCreateInfo.pPoolSizes = pDescriptorPoolSizes;
		}

		VkDescriptorPool *pDescriptorPool = (VkDescriptorPool*)NormalAllocator::Get()->Allocate(sizeof(VkDescriptorPool));
		RE_ASSERT(vkCreateDescriptorPool(pDevice, &descriptorPoolCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pDescriptorPool) == VK_SUCCESS, "Failed to Create Descriptor Pool.");

		RHIGroupPool *groupPool = (RHIGroupPool*)NormalAllocator::Get()->Allocate(sizeof(VulkanGroupPool));
		::new (groupPool) VulkanGroupPool(pDescriptorPool, descriptor->pGroupLayout, m_pDevice);

		return groupPool;
	}

	RHIScissor* VulkanDevice::CreateScissor(RHIScissorDescriptor* descriptor)
	{
		VkRect2D *pScissor = (VkRect2D*)NormalAllocator::Get()->Allocate(sizeof(VkRect2D));
		pScissor->offset.x = descriptor->offsetX;
		pScissor->offset.y = descriptor->offsetY;
		pScissor->extent.width = descriptor->width;
		pScissor->extent.height = descriptor->height;

		VulkanScissor *scissor = (VulkanScissor*)NormalAllocator::Get()->Allocate(sizeof(VulkanScissor));
		::new (scissor) VulkanScissor(pScissor);

		return scissor;
	}

	RHIViewport* VulkanDevice::CreateViewport(RHIViewportDescriptor* descriptor)
	{
		VkViewport *pViewport = (VkViewport*)NormalAllocator::Get()->Allocate(sizeof(VkViewport));
		pViewport->x = descriptor->x;
		pViewport->y = descriptor->y;
		pViewport->width = descriptor->width;
		pViewport->height = descriptor->height;
		pViewport->minDepth = descriptor->minDepth;
		pViewport->maxDepth = descriptor->maxDepth;

		VulkanViewport *viewport = (VulkanViewport*)NormalAllocator::Get()->Allocate(sizeof(VulkanViewport));
		::new (viewport) VulkanViewport(pViewport);

		return viewport;
	}

	void VulkanDevice::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
	{
		VkWriteDescriptorSet *pWriteDescriptorSets = (VkWriteDescriptorSet*)NormalAllocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkWriteDescriptorSet));
		WEngine::WArray<VkDescriptorBufferInfo*> pDescriptorBufferInfos(descriptor->bindingCount);
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			pDescriptorBufferInfos[i] = (VkDescriptorBufferInfo*)NormalAllocator::Get()->Allocate(descriptor->pBindingDescriptors[i].bufferResourceCount * sizeof(VkDescriptorBufferInfo));
			for (unsigned int j = 0; j < descriptor->pBindingDescriptors[i].bufferResourceCount; ++j)
			{
				::new (pDescriptorBufferInfos[i] + j) VkDescriptorBufferInfo();
				pDescriptorBufferInfos[i][j].buffer = *static_cast<VulkanUniformBuffer*>((descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->pBuffer)->GetHandle();
				pDescriptorBufferInfos[i][j].offset = (descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->offset;
				pDescriptorBufferInfos[i][j].range = (descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->range;
			}

			::new (pWriteDescriptorSets + i) VkWriteDescriptorSet();
			pWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteDescriptorSets[i].descriptorCount = descriptor->pBindingResources[i].count;
			pWriteDescriptorSets[i].descriptorType = WEngine::ToVulkan(descriptor->pBindingResources[i].type);
			pWriteDescriptorSets[i].dstSet = *static_cast<VulkanGroup*>(descriptor->pGroup)->GetHandle();
			pWriteDescriptorSets[i].dstBinding = descriptor->pBindingResources[i].bindingSlot;
			pWriteDescriptorSets[i].pBufferInfo = pDescriptorBufferInfos[i];
		}
		vkUpdateDescriptorSets(pDevice, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			for (unsigned int j = 0; j < descriptor->pBindingDescriptors[i].bufferResourceCount; ++j)
			{
				pDescriptorBufferInfos[i][j].~VkDescriptorBufferInfo();
			}
			(pWriteDescriptorSets + i)->~VkWriteDescriptorSet();
			NormalAllocator::Get()->Deallocate(pDescriptorBufferInfos[i]);
		}

		NormalAllocator::Get()->Deallocate(pWriteDescriptorSets);
	}

	void VulkanDevice::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
	{
		VkWriteDescriptorSet *pWriteDescriptorSets = (VkWriteDescriptorSet*)NormalAllocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkWriteDescriptorSet));
		WEngine::WArray<VkDescriptorImageInfo*> pDescriptorImageInfos(descriptor->bindingCount, nullptr);
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			pDescriptorImageInfos[i] = (VkDescriptorImageInfo*)NormalAllocator::Get()->Allocate(descriptor->textureResourceCount * sizeof(VkDescriptorImageInfo));
			for (int j = 0; j < descriptor->textureResourceCount; ++j)
			{
				::new (pDescriptorImageInfos[i] + j) VkDescriptorImageInfo();
				TextureResourceInfo *info = descriptor->pTextureInfo + j;
				pDescriptorImageInfos[i][j].imageView = info->pTextureView ? *static_cast<VulkanTextureView*>(info->pTextureView)->GetHandle() : nullptr;
				pDescriptorImageInfos[i][j].sampler = info->pSampler ? *static_cast<VulkanSampler*>(info->pSampler)->GetHandle() : nullptr;
				pDescriptorImageInfos[i][j].imageLayout = WEngine::ToVulkan(info->layout);
			}

			::new (pWriteDescriptorSets + i) VkWriteDescriptorSet();
			pWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteDescriptorSets[i].descriptorCount = descriptor->pBindingResources[i].count;
			pWriteDescriptorSets[i].descriptorType = WEngine::ToVulkan(descriptor->pBindingResources[i].type);
			pWriteDescriptorSets[i].dstSet = *static_cast<VulkanGroup*>(descriptor->pGroup)->GetHandle();
			pWriteDescriptorSets[i].dstBinding = descriptor->pBindingResources[i].bindingSlot;
			pWriteDescriptorSets[i].pImageInfo = pDescriptorImageInfos[i];
		}
		vkUpdateDescriptorSets(*m_pDevice, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			for (unsigned int j = 0; j < descriptor->textureResourceCount; ++j)
			{
				pDescriptorImageInfos[i][j].~VkDescriptorImageInfo();
			}
			(pWriteDescriptorSets + i)->~VkWriteDescriptorSet();
			NormalAllocator::Get()->Deallocate(pDescriptorImageInfos[i]);
		}

		NormalAllocator::Get()->Deallocate(pWriteDescriptorSets);
	}

	WEngine::WArray<RHISemaphore*> VulkanDevice::GetSemaphore(unsigned int count)
	{
		VkSemaphore *pSemaphore = (VkSemaphore*)NormalAllocator::Get()->Allocate(count * sizeof(VkSemaphore));
		VulkanSemaphore *semaphore = (VulkanSemaphore*)NormalAllocator::Get()->Allocate(count * sizeof(VulkanSemaphore));
		WEngine::WArray<RHISemaphore*> semaphores(count);
		
		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.flags = VK_PIPELINE_STAGE_HOST_BIT;
		for (int i = 0; i < count; ++i)
		{
			RE_ASSERT(vkCreateSemaphore(*m_pDevice, &semaphoreCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pSemaphore + i) == VK_SUCCESS, "Failed to Create Semaphore.");
			::new (semaphore + i) VulkanSemaphore(pSemaphore + i);
			semaphores[i] = semaphore + i;
		}

		return semaphores;
	}

	void VulkanDevice::WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll)
	{
		WEngine::WArray<VkFence> fences;
		fences.Reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.Push(*static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		vkWaitForFences(*m_pDevice, count, fences.GetData(), waitForAll, (std::numeric_limits<uint64_t>::max)());
	}

	void VulkanDevice::ResetFences(RHIFence* pFences, unsigned int count)
	{
		WEngine::WArray<VkFence> fences;
		fences.Reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.Push(*static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		RE_ASSERT(vkResetFences(*m_pDevice, count, fences.GetData()) == VK_SUCCESS, "Failed to Reset Fences.");
	}

	int VulkanDevice::GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore)
	{
		unsigned int index;
		VkResult result = vkAcquireNextImageKHR(*m_pDevice, *static_cast<VulkanSwapchain*>(swapchain)->GetHandle(), (std::numeric_limits<uint64_t>::max)(), *static_cast<VulkanSemaphore*>(semaphore)->GetHandle(), VK_NULL_HANDLE, &index);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window::cur_window->IsSizeChanged())
		{
			vkDeviceWaitIdle(*m_pDevice);
			return -1;
		}
		return index;
	}

	void VulkanDevice::Wait()
	{
		vkDeviceWaitIdle(*m_pDevice);
	}

}
