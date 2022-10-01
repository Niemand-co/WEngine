#include "pch.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"
#include "Platform/Vulkan/Public/VulkanSwapchain.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Platform/Vulkan/Public/VulkanShader.h"
#include "Platform/Vulkan/Public/VulkanRenderPass.h"
#include "Platform/Vulkan/Public/VulkanPipelineStateObject.h"
#include "Platform/Vulkan/Public/VulkanRenderTarget.h"
#include "Platform/Vulkan/Public/VulkanTextureView.h"
#include "Platform/Vulkan/Public/VulkanTexture.h"
#include "Platform/Vulkan/Public/VulkanRenderTarget.h"
#include "Platform/Vulkan/Public/VulkanBuffer.h"
#include "Platform/Vulkan/Public/VulkanSemaphore.h"
#include "Platform/Vulkan/Public/VulkanFence.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHIShaderDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderTargetDescriptor.h"
#include "Render/Descriptor/Public/RHIBufferDescriptor.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VkDevice *device, std::vector<QueueStack> stacks)
	{
		m_device = device;
		m_queues = stacks;
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
		vkGetDeviceQueue(*m_device, m_queues[queueFamilyID].index, 0, pQueue);

		RHIQueue *queue = (RHIQueue*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanQueue));
		:: new (queue) VulkanQueue(pQueue, queueFamilyID, m_device);

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
		RE_ASSERT(vkCreateSwapchainKHR(*m_device, &swapchainCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSwapchain) == VK_SUCCESS, "Failed to Create Swapchain.");

		unsigned int imageCount = 0;
		vkGetSwapchainImagesKHR(*m_device, *pSwapchain, &imageCount, nullptr);
		VkImage *images = (VkImage*)WEngine::Allocator::Get()->Allocate(imageCount * sizeof(VkImage));
		vkGetSwapchainImagesKHR(*m_device, *pSwapchain, &imageCount, images);

		RHISwapchain *swapchain = (RHISwapchain*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanSwapchain));
		::new (swapchain) VulkanSwapchain(pSwapchain, images, imageCount, m_device, 0);

		return swapchain;
	}

	void VulkanDevice::RecreateSwapchain(RHISwapchain* swapchain, RHISwapchainDescriptor* descriptor)
	{
		vkDestroySwapchainKHR(*m_device, *static_cast<VulkanSwapchain*>(swapchain)->GetHandle(), nullptr);

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

		RE_ASSERT(vkCreateSwapchainKHR(*m_device, &swapchainCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), static_cast<VulkanSwapchain*>(swapchain)->GetHandle()) == VK_SUCCESS, "Failed to Recreate Swapchain.");
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
			vkCreateFence(*m_device, &fenceCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), fence + i);
			::new (pFences + i) VulkanFence(fence + i);
			fences[i] = pFences + i;
		}


		return fences;
	}

	RHIShader* VulkanDevice::CreateShader(RHIShaderDescriptor *descriptor)
	{
		VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = descriptor->codeSize;
		shaderModuleCreateInfo.pCode = descriptor->pCode;

		VkShaderModule *pShaderModule = (VkShaderModule*)WEngine::Allocator::Get()->Allocate(sizeof(VkShaderModule));
		vkCreateShaderModule(*m_device, &shaderModuleCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pShaderModule);

		RHIShader *shader = (RHIShader*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanShader));
		:: new (shader) VulkanShader(pShaderModule, descriptor->shaderStage, descriptor->entryName);

		return shader;
	}

	RHIRenderPass* VulkanDevice::CreateRenderPass(RHIRenderPassDescriptor* descriptor)
	{
		VkAttachmentDescription attachmentDescription = {};
		attachmentDescription.format = WEngine::ToVulkan(descriptor->attachmentFormat);
		attachmentDescription.samples = WEngine::ToVulkan(descriptor->sampleCount);
		attachmentDescription.loadOp = WEngine::ToVulkan(descriptor->attachmentLoadOP);
		attachmentDescription.storeOp = WEngine::ToVulkan(descriptor->attachmentStoreOP);
		attachmentDescription.stencilLoadOp = WEngine::ToVulkan(descriptor->stencilLoadOP);
		attachmentDescription.stencilStoreOp = WEngine::ToVulkan(descriptor->stencilStoreOP);
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentReference = {};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &attachmentReference;

		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = descriptor->attachmentCount;
		renderPassCreateInfo.pAttachments = &attachmentDescription;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDescription;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &subpassDependency;

		VkRenderPass *pRenderPass = (VkRenderPass*)WEngine::Allocator::Get()->Allocate(sizeof(VkRenderPass));
		RE_ASSERT(vkCreateRenderPass(*m_device, &renderPassCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pRenderPass) == VK_SUCCESS, "Failed to Create Render Pass.");

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

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
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

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
		multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleStateCreateInfo.minSampleShading = 1.0f;
		multisampleStateCreateInfo.pSampleMask = nullptr;
		multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
		colorBlendAttachmentState.blendEnable = descriptor->blendDescriptor->blendEnabled;
		colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachmentState.srcColorBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->colorSrcFactor);
		colorBlendAttachmentState.dstColorBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->colorDstFactor);
		colorBlendAttachmentState.colorBlendOp = WEngine::ToVulkan(descriptor->blendDescriptor->colorBlendOP);
		colorBlendAttachmentState.srcAlphaBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->alphaSrcFactor);
		colorBlendAttachmentState.dstAlphaBlendFactor = WEngine::ToVulkan(descriptor->blendDescriptor->alphaDstFactor);
		colorBlendAttachmentState.alphaBlendOp = WEngine::ToVulkan(descriptor->blendDescriptor->alphaBlendOP);

		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
		depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateCreateInfo.stencilTestEnable = false;
		depthStencilStateCreateInfo.depthTestEnable = descriptor->depthStencilDescriptor->depthTestEnabled;
		depthStencilStateCreateInfo.depthCompareOp = WEngine::ToVulkan(descriptor->depthStencilDescriptor->depthCompareOP);
		depthStencilStateCreateInfo.depthWriteEnable = descriptor->depthStencilDescriptor->depthWriteEnabled;
		depthStencilStateCreateInfo.depthBoundsTestEnable = descriptor->depthStencilDescriptor->depthBoundsTest;
		depthStencilStateCreateInfo.maxDepthBounds = descriptor->depthStencilDescriptor->maxDepth;
		depthStencilStateCreateInfo.minDepthBounds = descriptor->depthStencilDescriptor->minDepth;

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
		colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateCreateInfo.attachmentCount = 1;
		colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
		colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
		colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

		VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = 2;
		dynamicStateCreateInfo.pDynamicStates = dynamicStates;

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutCreateInfo.setLayoutCount = 0;
		layoutCreateInfo.pSetLayouts = nullptr;
		layoutCreateInfo.pushConstantRangeCount = 0;
		layoutCreateInfo.pPushConstantRanges = nullptr;
		VkPipelineLayout *layout = (VkPipelineLayout*)WEngine::Allocator::Get()->Allocate(sizeof(VkPipelineLayout));
		RE_ASSERT(vkCreatePipelineLayout(*m_device, &layoutCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), layout) == VK_SUCCESS, "Failed to Create Pipeline Layout.");

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
		graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCreateInfo.stageCount = shaderStageCreateInfos.size();
		graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.data();
		graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
		graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
		graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
		graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
		graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
		graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		graphicsPipelineCreateInfo.layout = *layout;
		graphicsPipelineCreateInfo.renderPass = *static_cast<VulkanRenderPass*>(descriptor->renderPass)->GetHandle();
		graphicsPipelineCreateInfo.subpass = 0;
		graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		//graphicsPipelineCreateInfo.basePipelineIndex = -1;

		VkPipeline *pPipeline = (VkPipeline*)WEngine::Allocator::Get()->Allocate(sizeof(VkPipeline));
		RE_ASSERT(vkCreateGraphicsPipelines(*m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pPipeline) == VK_SUCCESS, "Failed to Create Pipeline.");

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
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkImage *image = (VkImage*)WEngine::Allocator::Get()->Allocate(sizeof(VkImage));
		vkCreateImage(*m_device, &imageCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), image);

		return nullptr;
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
		RE_ASSERT(vkCreateFramebuffer(*m_device, &framebufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), framebuffer) == VK_SUCCESS, "Failed to Create Framebuffer.");

		RHIRenderTarget *renderTarget = (RHIRenderTarget*)WEngine::Allocator::Get()->Allocate(sizeof(VulkanRenderTarget));
		::new (renderTarget) VulkanRenderTarget(framebuffer, descriptor->width, descriptor->height, m_device);

		return renderTarget;
	}

	RHIBuffer* VulkanDevice::CreateBuffer(RHIBufferDescriptor* descriptor)
	{
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.size = descriptor->size;

		VkBuffer* buffer = (VkBuffer*)WEngine::Allocator::Get()->Allocate(sizeof(VkBuffer));
		RE_ASSERT(vkCreateBuffer(*m_device, &bufferCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), buffer) == VK_SUCCESS, "Failed to Create Buffer.");

		return new VulkanBuffer(buffer, m_device);
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
			RE_ASSERT(vkCreateSemaphore(*m_device, &semaphoreCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pSemaphore + i) == VK_SUCCESS, "Failed to Create Semaphore.");
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

		vkWaitForFences(*m_device, count, fences.data(), waitForAll, (std::numeric_limits<uint64_t>::max)());
	}

	void VulkanDevice::ResetFences(RHIFence* pFences, unsigned int count)
	{
		std::vector<VkFence> fences;
		fences.reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.push_back(*static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		RE_ASSERT(vkResetFences(*m_device, count, fences.data()) == VK_SUCCESS, "Failed to Reset Fences.");
	}

	int VulkanDevice::GetNextImage(RHISwapchain *swapchain, RHISemaphore *semaphore)
	{
		unsigned int index;
		VkResult result = vkAcquireNextImageKHR(*m_device, *static_cast<VulkanSwapchain*>(swapchain)->GetHandle(), (std::numeric_limits<uint64_t>::max)(), *static_cast<VulkanSemaphore*>(semaphore)->GetHandle(), VK_NULL_HANDLE, &index);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window::cur_window->IsSizeChanged())
		{
			vkDeviceWaitIdle(*m_device);
			return -1;
		}
		return index;
	}

	VkDevice* VulkanDevice::GetHandle()
	{
		return m_device;
	}

}
