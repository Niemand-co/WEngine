#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Vertex.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VkDevice *device, VulkanGPU *pGPU, std::vector<QueueStack> stacks)
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
		for(unsigned int i = 0; i < m_queues.size(); ++i)
			if(m_queues[i].type == type)
				return m_queues[i].count;
		return 0;
	}

	RHIQueue* VulkanDevice::GetQueue(RHIQueueType type, unsigned int count)
	{
		VkQueue *pQueue = (VkQueue*)WEngine::Allocator::Get()->Allocate(sizeof(VkQueue));
		unsigned int queueFamilyID = 0;
		for(; queueFamilyID < m_queues.size(); ++queueFamilyID)
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

		VkSwapchainKHR *pSwapchain = (VkSwapchainKHR*)WEngine::Allocator::Get()->Allocate(sizeof(VkSwapchainKHR));
		RE_ASSERT(vkCreateSwapchainKHR(*m_pDevice, &swapchainCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSwapchain) == VK_SUCCESS, "Failed to Create Swapchain.");

		RHISwapchain *swapchain = (RHISwapchain*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanSwapchain));
		::new (swapchain) VulkanSwapchain(pSwapchain, m_pDevice, 0);

		return swapchain;
	}

	std::vector<RHIFence*> VulkanDevice::CreateFence(unsigned int count)
	{
		VulkanFence *pFences = (VulkanFence*)WEngine::Allocator::Get()->Allocate(count * sizeof(VulkanFence));
		std::vector<RHIFence*> fences(count);

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

		VkSubpassDescription *pSubpassDescriptions = (VkSubpassDescription*)WEngine::Allocator::Get()->Allocate(descriptor->subpassCount * sizeof(VkSubpassDescription));
		VkAttachmentReference *pAttachmentReferences = (VkAttachmentReference*)WEngine::Allocator::Get()->Allocate(descriptor->subpassCount * sizeof(VkAttachmentReference));
		VkSubpassDependency *pSubpassDependencies = (VkSubpassDependency*)WEngine::Allocator::Get()->Allocate(descriptor->subpassCount * sizeof(VkSubpassDependency));
		for (unsigned int i = 0; i < descriptor->subpassCount; ++i)
		{
			RHISubPassDescriptor *pSubpassDescriptor = descriptor->pSubPassDescriptors + i;
			::new (pAttachmentReferences + i) VkAttachmentReference();
			pAttachmentReferences[i].attachment = pSubpassDescriptor->attachmentIndex;
			pAttachmentReferences[i].layout = WEngine::ToVulkan(pSubpassDescriptor->attachmentLayout);
			::new (pSubpassDescriptions + i) VkSubpassDescription();
			pSubpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			pSubpassDescriptions[i].colorAttachmentCount = 1;
			pSubpassDescriptions[i].pColorAttachments = &pAttachmentReferences[i];
			::new (pSubpassDependencies + i) VkSubpassDependency();
			pSubpassDependencies[i].srcSubpass = pSubpassDescriptor->dependedPass >= 0 ? pSubpassDescriptor->dependedPass : VK_SUBPASS_EXTERNAL;
			pSubpassDependencies[i].dstSubpass = i;
			pSubpassDependencies[i].srcStageMask = pSubpassDescriptor->dependedStage;
			pSubpassDependencies[i].srcAccessMask = pSubpassDescriptor->dependedAccess;
			pSubpassDependencies[i].dstStageMask = pSubpassDescriptor->waitingStage;
			pSubpassDependencies[i].dstAccessMask = pSubpassDescriptor->waitingAccess;
		}

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = descriptor->attachmentCount;
		renderPassCreateInfo.pAttachments = pAttachmentDescriptions;
		renderPassCreateInfo.subpassCount = descriptor->subpassCount;
		renderPassCreateInfo.pSubpasses = pSubpassDescriptions;
		renderPassCreateInfo.dependencyCount = descriptor->subpassCount;
		renderPassCreateInfo.pDependencies = pSubpassDependencies;

		VkRenderPass *pRenderPass = (VkRenderPass*)WEngine::Allocator::Get()->Allocate(sizeof(VkRenderPass));
   		RE_ASSERT(vkCreateRenderPass(*m_pDevice, &renderPassCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pRenderPass) == VK_SUCCESS, "Failed to Create Render Pass.");

		RHIRenderPass *renderPass = (RHIRenderPass*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanRenderPass));
		::new (renderPass) VulkanRenderPass(pRenderPass);

		return renderPass;
	}

	RHIPipelineStateObject* VulkanDevice::CreatePipelineStateObject(RHIPipelineStateObjectDescriptor* descriptor)
	{

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(descriptor->shaderCount);
		for (unsigned int i = 0; i < descriptor->shaderCount; ++i)
		{
			RHIShader* shader = descriptor->pShader[i];
			shaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStageCreateInfos[i].stage = WEngine::ToVulkan(shader->GetStage());
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
			inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
		}

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		{
			viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		}

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
		{
			rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizationStateCreateInfo.lineWidth = 1.0f;
			rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
			rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
			depthStencilStateCreateInfo.stencilTestEnable = false;
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

		VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		{
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.dynamicStateCount = 2;
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
		}

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
		{
			graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			graphicsPipelineCreateInfo.stageCount = shaderStageCreateInfos.size();
			graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.data();
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
			graphicsPipelineCreateInfo.subpass = 0;
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
		imageCreateInfo.extent = { descriptor->width, descriptor->height };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.usage = descriptor->usage;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = WEngine::ToVulkan(descriptor->layout);
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = WEngine::ToVulkan(1);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkImage *image = (VkImage*)WEngine::Allocator::Get()->Allocate(sizeof(VkImage));
		vkCreateImage(*m_pDevice, &imageCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), image);

		RHITexture *pTexture = (RHITexture*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanTexture));
		::new (pTexture) VulkanTexture(image, m_pDevice);

		return pTexture;
	}

	RHIRenderTarget* VulkanDevice::CreateRenderTarget(RHIRenderTargetDescriptor* descriptor)
	{
		std::vector<VkImageView> views(descriptor->bufferCount);
		for (unsigned int i = 0; i < views.size(); ++i)
		{
			views[i] = *(static_cast<VulkanTextureView*>(descriptor->pBufferView[i])->GetHandle());
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.attachmentCount = views.size();
		framebufferCreateInfo.pAttachments = views.data();
		framebufferCreateInfo.renderPass = *static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
		framebufferCreateInfo.width = descriptor->width;
		framebufferCreateInfo.height = descriptor->height;
		framebufferCreateInfo.layers = 1;

		VkFramebuffer *framebuffer = (VkFramebuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkFramebuffer));
		RE_ASSERT(vkCreateFramebuffer(*m_pDevice, &framebufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), framebuffer) == VK_SUCCESS, "Failed to Create Framebuffer.");

		RHIRenderTarget *renderTarget = (RHIRenderTarget*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanRenderTarget));
		::new (renderTarget) VulkanRenderTarget(framebuffer, descriptor->width, descriptor->height, m_pDevice);

		return renderTarget;
	}

	RHIBuffer* VulkanDevice::CreateBuffer(RHIBufferDescriptor* descriptor)
	{
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.usage = descriptor->bufferType;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.size = descriptor->size;

		VkBuffer* pBuffer = (VkBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkBuffer));
		RE_ASSERT(vkCreateBuffer(*m_pDevice, &bufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pBuffer) == VK_SUCCESS, "Failed to Create Buffer.");

		unsigned int index = 0;
		GPUFeature feature = m_pGPU->GetFeature();
		for (; index < feature.memorySupports.size(); ++index)
		{
			if (feature.memorySupports[index]->type == MemoryType::LocalMemory && (feature.memorySupports[index]->properties & descriptor->memoryType) != 0)
			{
				break;
			}
		}
		RE_ASSERT(feature.memorySupports.size() > index, "No Suitable Memory Heap Exists.");

		RHIBuffer *buffer = (RHIBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanBuffer));
		::new (buffer) VulkanBuffer(pBuffer, m_pDevice, index, descriptor->size, descriptor->pData);

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
			pDescriptorSetLayoutBindings[i].stageFlags = WEngine::ToVulkan(resource->shaderStage);
			pDescriptorSetLayoutBindings[i].pImmutableSamplers = nullptr;
		}
		
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
		{
			descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutCreateInfo.bindingCount = descriptor->bindingCount;
			descriptorSetLayoutCreateInfo.pBindings = pDescriptorSetLayoutBindings;
		}

		VkDescriptorSetLayout *pDescriptorSetLayout = (VkDescriptorSetLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VkDescriptorSetLayout));
		RE_ASSERT(vkCreateDescriptorSetLayout(*m_pDevice, &descriptorSetLayoutCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pDescriptorSetLayout) == VK_SUCCESS, "Failed to Create Descriptor Set Layout.");

		RHIGroupLayout *groupLayout = (RHIGroupLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanGroupLayout));
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

	void VulkanDevice::UpdateResourceToGroup(RHIUpdateResourceDescriptor* descriptor)
	{
		VkWriteDescriptorSet* pWriteDescriptorSets = (VkWriteDescriptorSet*)WEngine::Allocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkWriteDescriptorSet));
		VkDescriptorBufferInfo *pDescriptorBufferInfo = (VkDescriptorBufferInfo*)WEngine::Allocator::Get()->Allocate(descriptor->bindingCount * sizeof(VkDescriptorBufferInfo));
		for (unsigned int i = 0; i < descriptor->bindingCount; ++i)
		{
			::new (pDescriptorBufferInfo + i) VkDescriptorBufferInfo();
			pDescriptorBufferInfo[i].buffer = *static_cast<VulkanBuffer*>(descriptor->pBuffer)->GetHandle();
			pDescriptorBufferInfo[i].offset = descriptor->pOffsets[i];
			pDescriptorBufferInfo[i].range = descriptor->pSize[i];

			::new (pWriteDescriptorSets + i) VkWriteDescriptorSet();
			pWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteDescriptorSets[i].descriptorCount = 1;//descriptor->pBindingResources[i].count;
			pWriteDescriptorSets[i].descriptorType = WEngine::ToVulkan(descriptor->pBindingResources[i].type);
			pWriteDescriptorSets[i].dstSet = *static_cast<VulkanGroup*>(descriptor->pGroup)->GetHandle();
			pWriteDescriptorSets[i].dstBinding = descriptor->pBindingResources[i].bindingSlot;
			pWriteDescriptorSets[i].pBufferInfo = pDescriptorBufferInfo + i;
		}
		vkUpdateDescriptorSets(*m_pDevice, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);
	}

	std::vector<RHISemaphore*> VulkanDevice::GetSemaphore(unsigned int count)
	{
		VkSemaphore *pSemaphore = (VkSemaphore*)WEngine::Allocator::Get()->Allocate(count * sizeof(VkSemaphore));
		VulkanSemaphore *semaphore = (VulkanSemaphore*)WEngine::Allocator::Get()->Allocate(count * sizeof(VulkanSemaphore));
		std::vector<RHISemaphore*> semaphores(count);
		
		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
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
		std::vector<VkFence> fences;
		fences.reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.push_back(*static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		vkWaitForFences(*m_pDevice, count, fences.data(), waitForAll, (std::numeric_limits<uint64_t>::max)());
	}

	void VulkanDevice::ResetFences(RHIFence* pFences, unsigned int count)
	{
		std::vector<VkFence> fences;
		fences.reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.push_back(*static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		RE_ASSERT(vkResetFences(*m_pDevice, count, fences.data()) == VK_SUCCESS, "Failed to Reset Fences.");
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

	VkDevice* VulkanDevice::GetHandle()
	{
		return m_pDevice;
	}

}
