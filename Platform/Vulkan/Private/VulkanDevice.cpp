#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Vertex.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VkDevice *device, VulkanGPU *pGPU, WEngine::WArray<QueueStack> stacks)
	{
		m_pDevice = device;
		m_queues = stacks;
		m_pGPU = pGPU;
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
		VkQueue *pQueue = (VkQueue*)WEngine::Allocator::Get()->Allocate(sizeof(VkQueue));
		::new (pQueue) VkQueue();
		unsigned int queueFamilyID = 0;
		for(; queueFamilyID < m_queues.Size(); ++queueFamilyID)
			if(m_queues[queueFamilyID].type == type)
				break;
		vkGetDeviceQueue(*m_pDevice, m_queues[queueFamilyID].index, 0, pQueue);

		RHIQueue *queue = (RHIQueue*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanQueue));
		:: new (queue) VulkanQueue(pQueue, queueFamilyID, m_pDevice);

		return queue;
	}

	RHISwapchain* VulkanDevice::CreateSwapchain(RHISwapchainDescriptor* descriptor)
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = static_cast<VulkanSurface*>(descriptor->surface)->GetHandle();
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

		VkSwapchainKHR* pSwapchain = (VkSwapchainKHR*)WEngine::Allocator::Get()->Allocate(sizeof(VkSwapchainKHR));
		RE_ASSERT(vkCreateSwapchainKHR(*m_pDevice, &swapchainCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSwapchain) == VK_SUCCESS, "Failed to Create Swapchain.");

		RHISwapchain *swapchain = (RHISwapchain*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanSwapchain));
		::new (swapchain) VulkanSwapchain(pSwapchain, m_pDevice, 0);

		return swapchain;
	}

	WEngine::WArray<RHIFence*> VulkanDevice::CreateFence(unsigned int count)
	{
		VulkanFence *pFences = (VulkanFence*)WEngine::Allocator::Get()->Allocate(count * sizeof(VulkanFence));
		WEngine::WArray<RHIFence*> fences(count);

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkFence *fence = (VkFence*)WEngine::Allocator::Get()->Allocate(count * sizeof(VkFence));
		for (unsigned int i = 0; i < count; ++i)
		{
			vkCreateFence(*m_pDevice, &fenceCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), fence + i);
			::new (pFences + i) VulkanFence(fence + i);
			fences[i] = pFences + i;
		}


		return fences;
	}

	RHIEvent* VulkanDevice::GetEvent()
	{
		VkEventCreateInfo eventCreateInfo = {};
		{
			eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		}

		VkEvent* pEvent = (VkEvent*)WEngine::Allocator::Get()->Allocate(sizeof(VkEvent));
		vkCreateEvent(*m_pDevice, &eventCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pEvent);
		
		RHIEvent *event = (RHIEvent*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanEvent));
		::new (event) VulkanEvent(pEvent);

		return event;
	}

	RHIShader* VulkanDevice::CreateShader(RHIShaderDescriptor *descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule *pShaderModule = (VkShaderModule*)WEngine::Allocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_pDevice, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pShaderModule);

		RHIShader *shader = (RHIShader*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanShader));
		:: new (shader) VulkanShader(pShaderModule, descriptor->shaderStage, descriptor->entryName);

		return shader;
	}

	RHIRenderPass* VulkanDevice::CreateRenderPass(RHIRenderPassDescriptor* descriptor)
	{
		VkAttachmentDescription *pAttachmentDescriptions = (VkAttachmentDescription*)WEngine::Allocator::Get()->Allocate(descriptor->attachmentCount * sizeof(VkAttachmentDescription));
		for (unsigned int i = 0; i < descriptor->attachmentCount; ++i)
		{
			RHIAttachmentDescriptor *pAttachmentDescriptor = descriptor->pAttachmentDescriptors + i;
			::new (pAttachmentDescriptions + i) VkAttachmentDescription();
			pAttachmentDescriptions[i].format = WEngine::ToVulkan(pAttachmentDescriptor->attachmentFormat);
			pAttachmentDescriptions[i].samples = WEngine::ToVulkan(pAttachmentDescriptor->sampleCount);
			pAttachmentDescriptions[i].loadOp = WEngine::ToVulkan(pAttachmentDescriptor->attachmentLoadOP);
			pAttachmentDescriptions[i].storeOp = WEngine::ToVulkan(pAttachmentDescriptor->attachmentStoreOP);
			pAttachmentDescriptions[i].stencilLoadOp = WEngine::ToVulkan(pAttachmentDescriptor->stencilLoadOP);
			pAttachmentDescriptions[i].stencilStoreOp = WEngine::ToVulkan(pAttachmentDescriptor->stencilStoreOP);
			pAttachmentDescriptions[i].initialLayout = WEngine::ToVulkan(pAttachmentDescriptor->initialLayout);
			pAttachmentDescriptions[i].finalLayout = WEngine::ToVulkan(pAttachmentDescriptor->finalLayout);
		}

		WEngine::WArray<VkSubpassDescription> pSubpassDescriptions(descriptor->subpassCount);
		WEngine::WArray<VkAttachmentReference*> pColorAttachmentReferences(descriptor->subpassCount);
		WEngine::WArray<VkAttachmentReference*> pDepthAttachmentReferences(descriptor->subpassCount, nullptr);
		WEngine::WArray<VkAttachmentReference*> pInputAttachmentReferences(descriptor->subpassCount);

		for (unsigned int i = 0; i < descriptor->subpassCount; ++i)
		{
			RHISubPassDescriptor *pSubpassDescriptor = descriptor->pSubPassDescriptors + i;

			pColorAttachmentReferences[i] = (VkAttachmentReference*)WEngine::Allocator::Get()->Allocate(pSubpassDescriptor->colorAttachmentCount * sizeof(VkAttachmentReference));
			for (unsigned int j = 0; j < pSubpassDescriptor->colorAttachmentCount; ++j)
			{
				::new (pColorAttachmentReferences[i] + j) VkAttachmentReference();
				pColorAttachmentReferences[i][j].attachment = (pSubpassDescriptor->pColorAttachments + j)->attachmentIndex;
				pColorAttachmentReferences[i][j].layout = WEngine::ToVulkan((pSubpassDescriptor->pColorAttachments + j)->attachmentLayout);
			}

			if (pSubpassDescriptor->pDepthStencilAttachment != nullptr)
			{
				pDepthAttachmentReferences[i] = (VkAttachmentReference*)WEngine::Allocator::Get()->Allocate(sizeof(VkAttachmentReference));
				::new (pDepthAttachmentReferences[i]) VkAttachmentReference();
				pDepthAttachmentReferences[i]->attachment = pSubpassDescriptor->pDepthStencilAttachment->attachmentIndex;
				pDepthAttachmentReferences[i]->layout = WEngine::ToVulkan(pSubpassDescriptor->pDepthStencilAttachment->attachmentLayout);
			}

			pInputAttachmentReferences[i] = (VkAttachmentReference*)WEngine::Allocator::Get()->Allocate(pSubpassDescriptor->inputAttachmentCount * sizeof(VkAttachmentReference));
			for (unsigned int j = 0; j < pSubpassDescriptor->inputAttachmentCount; ++j)
			{
				::new (pInputAttachmentReferences[i] + j) VkAttachmentReference();
				pInputAttachmentReferences[i][j].attachment = (pSubpassDescriptor->pInputAttachments + j)->attachmentIndex;
				pInputAttachmentReferences[i][j].layout = WEngine::ToVulkan((pSubpassDescriptor->pInputAttachments + j)->attachmentLayout);
			}

			pSubpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			pSubpassDescriptions[i].colorAttachmentCount = pSubpassDescriptor->colorAttachmentCount;
			pSubpassDescriptions[i].pColorAttachments = pColorAttachmentReferences[i];
			pSubpassDescriptions[i].pDepthStencilAttachment = pDepthAttachmentReferences[i];
			pSubpassDescriptions[i].inputAttachmentCount = pSubpassDescriptor->inputAttachmentCount;
			pSubpassDescriptions[i].pInputAttachments = pInputAttachmentReferences[i];


		}

		WEngine::WArray<VkSubpassDependency> pSubpassDependencies(descriptor->dependencyCount);
		for (unsigned int i = 0; i < descriptor->dependencyCount; ++i)
		{
			RHISubPassDependencyDescriptor *dependencyDescriptor = descriptor->pDependencyDescriptors + i;
			pSubpassDependencies[i].srcSubpass = dependencyDescriptor->dependedPass >= 0 ? dependencyDescriptor->dependedPass : VK_SUBPASS_EXTERNAL;
			pSubpassDependencies[i].dstSubpass = dependencyDescriptor->waitingPass >= 0 ? dependencyDescriptor->waitingPass : VK_SUBPASS_EXTERNAL;
			pSubpassDependencies[i].srcStageMask = dependencyDescriptor->dependedStage;
			pSubpassDependencies[i].srcAccessMask = dependencyDescriptor->dependedAccess;
			pSubpassDependencies[i].dstStageMask = dependencyDescriptor->waitingStage;
			pSubpassDependencies[i].dstAccessMask = dependencyDescriptor->waitingAccess;
		}

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = descriptor->attachmentCount;
		renderPassCreateInfo.pAttachments = pAttachmentDescriptions;
		renderPassCreateInfo.subpassCount = descriptor->subpassCount;
		renderPassCreateInfo.pSubpasses = pSubpassDescriptions.GetData();
		renderPassCreateInfo.dependencyCount = descriptor->dependencyCount;
		renderPassCreateInfo.pDependencies = pSubpassDependencies.GetData();

		VkRenderPass *pRenderPass = (VkRenderPass*)WEngine::Allocator::Get()->Allocate(sizeof(VkRenderPass));
   		RE_ASSERT(vkCreateRenderPass(*m_pDevice, &renderPassCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pRenderPass) == VK_SUCCESS, "Failed to Create Render Pass.");

		RHIRenderPass *renderPass = (RHIRenderPass*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanRenderPass));
		::new (renderPass) VulkanRenderPass(pRenderPass);

		return renderPass;
	}

	RHIPipelineStateObject* VulkanDevice::CreatePipelineStateObject(RHIPipelineStateObjectDescriptor* descriptor)
	{

		WEngine::WArray<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(descriptor->shaderCount);
		for (unsigned int i = 0; i < descriptor->shaderCount; ++i)
		{
			RHIShader* shader = descriptor->pShader[i];
			shaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfos[i].stage = (VkShaderStageFlagBits)shader->GetStage();
			shaderStageCreateInfos[i].module = *static_cast<VulkanShader*>(shader)->GetShaderModule();
			shaderStageCreateInfos[i].pName = shader->GetEntry();
		}

		VkVertexInputBindingDescription vertexInputBindgDescription = {};
		{
			vertexInputBindgDescription.binding = descriptor->vertexDescriptor->bindingDescription->slot;
			vertexInputBindgDescription.stride = descriptor->vertexDescriptor->bindingDescription->stride;
			vertexInputBindgDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}
		VkVertexInputAttributeDescription *pVertexInputAttributeDescriptions = (VkVertexInputAttributeDescription*)WEngine::Allocator::Get()->Allocate(descriptor->vertexDescriptor->attributeDescriptionCount * sizeof(VkVertexInputAttributeDescription));
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
			graphicsPipelineCreateInfo.stageCount = shaderStageCreateInfos.Size();
			graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.GetData();
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

		VkPipeline *pPipeline = (VkPipeline*)WEngine::Allocator::Get()->Allocate(sizeof(VkPipeline));
		RE_ASSERT(vkCreateGraphicsPipelines(*m_pDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pPipeline) == VK_SUCCESS, "Failed to Create Pipeline.");

		for (unsigned int i = 0; i < descriptor->vertexDescriptor->attributeDescriptionCount; ++i)
		{
			pVertexInputAttributeDescriptions[i].~VkVertexInputAttributeDescription();
		}
		WEngine::Allocator::Get()->Deallocate(pVertexInputAttributeDescriptions);

		RHIPipelineStateObject *pipeline = (RHIPipelineStateObject*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanAllocator));
		::new (pipeline) VulkanPipelineStateObject(pPipeline);

		return pipeline;
	}

	RHITexture* VulkanDevice::CreateTexture(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, 1 };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.usage = descriptor->usage;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = WEngine::ToVulkan(descriptor->layout);
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = WEngine::ToVulkan(1);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

		VkImage *image = (VkImage*)WEngine::Allocator::Get()->Allocate(sizeof(VkImage));
		vkCreateImage(*m_pDevice, &imageCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), image);

		VkMemoryRequirements *memoryRequirements = (VkMemoryRequirements*)WEngine::Allocator::Get()->Allocate(sizeof(VkMemoryRequirements));
		::new (memoryRequirements) VkMemoryRequirements();
		vkGetImageMemoryRequirements(*m_pDevice, *image, memoryRequirements);

		unsigned int index = 0;
		GPUFeature feature = m_pGPU->GetFeature();
		for (; index < feature.memorySupports.Size(); ++index)
		{
			if((memoryRequirements->memoryTypeBits & 1) && (feature.memorySupports[index]->properties & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT )
				break;
			memoryRequirements->memoryTypeBits >>= 1;
		}

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		{
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.allocationSize = memoryRequirements->size;
			memoryAllocateInfo.memoryTypeIndex = index;
		}
		VkDeviceMemory *pMemory = (VkDeviceMemory*)WEngine::Allocator::Get()->Allocate(sizeof(VkDeviceMemory));
		vkAllocateMemory(*m_pDevice, &memoryAllocateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pMemory);

		vkBindImageMemory(*m_pDevice, *image, *pMemory, 0);

		return new VulkanTexture(image, memoryRequirements, pMemory, m_pDevice);
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

		VkSampler *pSampler = (VkSampler*)WEngine::Allocator::Get()->Allocate(sizeof(VkSampler));
		RE_ASSERT(vkCreateSampler(*m_pDevice, &samplerCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSampler) == VK_SUCCESS, "Failed to Create Sampler.");
		
		VulkanSampler *sampler = (VulkanSampler*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanSampler));
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

		VkFramebuffer *framebuffer = (VkFramebuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkFramebuffer));
		RE_ASSERT(vkCreateFramebuffer(*m_pDevice, &framebufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), framebuffer) == VK_SUCCESS, "Failed to Create Framebuffer.");

		VulkanRenderTarget *renderTarget = (VulkanRenderTarget*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanRenderTarget));
		::new (renderTarget) VulkanRenderTarget(framebuffer, descriptor->width, descriptor->height, m_pDevice);

		return renderTarget;
	}

	RHIVertexBuffer* VulkanDevice::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		
		return nullptr;
	}

	RHIIndexBuffer* VulkanDevice::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
	{
		return nullptr;
	}

	RHIUniformBuffer* VulkanDevice::CreateUniformBuffer(RHIBufferDescriptor* descriptor)
	{
		return nullptr;
	}

	RHIBuffer* VulkanDevice::CreateBuffer(RHIBufferDescriptor* descriptor)
	{
		size_t bufferSize = descriptor->dataSize;
		if (descriptor->isDynamic)
		{
			size_t minUBOSize = m_pGPU->GetFeature().minUBOAlignment;
			bufferSize = (bufferSize + minUBOSize - 1) & ~(minUBOSize - 1);
		}

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.usage = descriptor->bufferType;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.size = bufferSize * descriptor->count;

		VkBuffer* pBuffer = (VkBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkBuffer));
		::new (pBuffer) VkBuffer();
		RE_ASSERT(vkCreateBuffer(*m_pDevice, &bufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pBuffer) == VK_SUCCESS, "Failed to Create Buffer.");

		unsigned int index = 0;
		GPUFeature feature = m_pGPU->GetFeature();
		for (; index < feature.memorySupports.Size(); ++index)
		{
			if (feature.memorySupports[index]->type == MemoryType::LocalMemory && (feature.memorySupports[index]->properties & descriptor->memoryType) == descriptor->memoryType )
			{
				break;
			}
		}
		RE_ASSERT(feature.memorySupports.Size() > index, "No Suitable Memory Heap Exists.");

		VulkanBuffer *buffer = (VulkanBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanBuffer));
		if(descriptor->isDynamic)
			::new (buffer) VulkanBuffer(pBuffer, m_pDevice, index, descriptor->isDynamic, bufferSize, descriptor->count);
		else
			::new (buffer) VulkanBuffer(pBuffer, m_pDevice, index, descriptor->isDynamic, descriptor->dataSize);

		return buffer;
	}

	RHIGroup* VulkanDevice::CreateResourceGroup(RHIGroupDescriptor* descriptor)
	{

		return nullptr;
	}

	RHIGroupLayout* VulkanDevice::CreateGroupLayout(RHIGroupLayoutDescriptor *descriptor)
	{
		VkDescriptorSetLayoutBinding *pDescriptorSetLayoutBindings = (VkDescriptorSetLayoutBinding*)WEngine::Allocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkDescriptorSetLayoutBinding));
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

		VkDescriptorSetLayout *pDescriptorSetLayout = (VkDescriptorSetLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VkDescriptorSetLayout));
		::new (pDescriptorSetLayout) VkDescriptorSetLayout();
		RE_ASSERT(vkCreateDescriptorSetLayout(*m_pDevice, &descriptorSetLayoutCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDescriptorSetLayout) == VK_SUCCESS, "Failed to Create Descriptor Set Layout.");

		VulkanGroupLayout *groupLayout = (VulkanGroupLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanGroupLayout));
		::new (groupLayout) VulkanGroupLayout(pDescriptorSetLayout, descriptor->bindingCount);

		groupLayout->bindingCount = descriptor->bindingCount;
		groupLayout->pBindingResources = descriptor->pBindingResources;
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			BindingResource* resource = (descriptor->pBindingResources) + i;
			pDescriptorSetLayoutBindings[i].~VkDescriptorSetLayoutBinding();
		}
		WEngine::Allocator::Get()->Deallocate(pDescriptorSetLayoutBindings);

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
		VkPipelineLayout* pLayout = (VkPipelineLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VkPipelineLayout));
		RE_ASSERT(vkCreatePipelineLayout(*m_pDevice, &layoutCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pLayout) == VK_SUCCESS, "Failed to Create Pipeline Layout.");

		RHIPipelineResourceLayout *pPipelineResourceLayout = (RHIPipelineResourceLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanPipelineResourceLayout));
		::new (pPipelineResourceLayout) VulkanPipelineResourceLayout(pLayout);

		return pPipelineResourceLayout;
	}

	RHIGroupPool* VulkanDevice::CreateGroupPool(RHIGroupPoolDescriptor* descriptor)
	{
		VkDescriptorPoolSize *pDescriptorPoolSizes = (VkDescriptorPoolSize*)WEngine::Allocator::Get()->Allocate(descriptor->pGroupLayout->bindingCount * sizeof(VkDescriptorPoolSize));
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

		VkDescriptorPool *pDescriptorPool = (VkDescriptorPool*)WEngine::Allocator::Get()->Allocate(sizeof(VkDescriptorPool));
		RE_ASSERT(vkCreateDescriptorPool(*m_pDevice, &descriptorPoolCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDescriptorPool) == VK_SUCCESS, "Failed to Create Descriptor Pool.");

		RHIGroupPool *groupPool = (RHIGroupPool*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanGroupPool));
		::new (groupPool) VulkanGroupPool(pDescriptorPool, descriptor->pGroupLayout, m_pDevice);

		return groupPool;
	}

	RHIScissor* VulkanDevice::CreateScissor(RHIScissorDescriptor* descriptor)
	{
		VkRect2D *pScissor = (VkRect2D*)WEngine::Allocator::Get()->Allocate(sizeof(VkRect2D));
		pScissor->offset.x = descriptor->offsetX;
		pScissor->offset.y = descriptor->offsetY;
		pScissor->extent.width = descriptor->width;
		pScissor->extent.height = descriptor->height;

		VulkanScissor *scissor = (VulkanScissor*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanScissor));
		::new (scissor) VulkanScissor(pScissor);

		return scissor;
	}

	RHIViewport* VulkanDevice::CreateViewport(RHIViewportDescriptor* descriptor)
	{
		VkViewport *pViewport = (VkViewport*)WEngine::Allocator::Get()->Allocate(sizeof(VkViewport));
		pViewport->x = descriptor->x;
		pViewport->y = descriptor->y;
		pViewport->width = descriptor->width;
		pViewport->height = descriptor->height;
		pViewport->minDepth = descriptor->minDepth;
		pViewport->maxDepth = descriptor->maxDepth;

		VulkanViewport *viewport = (VulkanViewport*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanViewport));
		::new (viewport) VulkanViewport(pViewport);

		return viewport;
	}

	void VulkanDevice::UpdateUniformResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
	{
		VkWriteDescriptorSet *pWriteDescriptorSets = (VkWriteDescriptorSet*)WEngine::Allocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkWriteDescriptorSet));
		WEngine::WArray<VkDescriptorBufferInfo*> pDescriptorBufferInfos(descriptor->bindingCount);
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			pDescriptorBufferInfos[i] = (VkDescriptorBufferInfo*)WEngine::Allocator::Get()->Allocate(descriptor->pBindingDescriptors[i].bufferResourceCount * sizeof(VkDescriptorBufferInfo));
			for (unsigned int j = 0; j < descriptor->pBindingDescriptors[i].bufferResourceCount; ++j)
			{
				::new (pDescriptorBufferInfos[i] + j) VkDescriptorBufferInfo();
				pDescriptorBufferInfos[i][j].buffer = *static_cast<VulkanBuffer*>((descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->pBuffer)->GetHandle();
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
		vkUpdateDescriptorSets(*m_pDevice, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			for (unsigned int j = 0; j < descriptor->pBindingDescriptors[i].bufferResourceCount; ++j)
			{
				pDescriptorBufferInfos[i][j].~VkDescriptorBufferInfo();
			}
			(pWriteDescriptorSets + i)->~VkWriteDescriptorSet();
			WEngine::Allocator::Get()->Deallocate(pDescriptorBufferInfos[i]);
		}

		WEngine::Allocator::Get()->Deallocate(pWriteDescriptorSets);
	}

	void VulkanDevice::UpdateTextureResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
	{
		VkWriteDescriptorSet *pWriteDescriptorSets = (VkWriteDescriptorSet*)WEngine::Allocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkWriteDescriptorSet));
		WEngine::WArray<VkDescriptorImageInfo*> pDescriptorImageInfos(descriptor->bindingCount, nullptr);
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			pDescriptorImageInfos[i] = (VkDescriptorImageInfo*)WEngine::Allocator::Get()->Allocate(descriptor->textureResourceCount * sizeof(VkDescriptorImageInfo));
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
			WEngine::Allocator::Get()->Deallocate(pDescriptorImageInfos[i]);
		}

		WEngine::Allocator::Get()->Deallocate(pWriteDescriptorSets);
	}

	WEngine::WArray<RHISemaphore*> VulkanDevice::GetSemaphore(unsigned int count)
	{
		VkSemaphore *pSemaphore = (VkSemaphore*)WEngine::Allocator::Get()->Allocate(count * sizeof(VkSemaphore));
		VulkanSemaphore *semaphore = (VulkanSemaphore*)WEngine::Allocator::Get()->Allocate(count * sizeof(VulkanSemaphore));
		WEngine::WArray<RHISemaphore*> semaphores(count);
		
		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.flags = VK_PIPELINE_STAGE_HOST_BIT;
		for (int i = 0; i < count; ++i)
		{
			RE_ASSERT(vkCreateSemaphore(*m_pDevice, &semaphoreCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSemaphore + i) == VK_SUCCESS, "Failed to Create Semaphore.");
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

	VkDevice* VulkanDevice::GetHandle()
	{
		return m_pDevice;
	}

}
