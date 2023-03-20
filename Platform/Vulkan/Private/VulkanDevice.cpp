#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Vertex.h"
#include "RHI/Public/RHIBuffer.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Public/Shader.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VulkanGPU *pInGPU, VkDeviceCreateInfo* pInfo, WEngine::WArray<QueueStack>& InQueueStack)
		: pGPU(pInGPU), m_queues(InQueueStack)
	{
		RE_ASSERT(vkCreateDevice(*pInGPU->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &pDevice) == VK_SUCCESS, "Failed to Create Device.");

		pMemoryManager = new VulkanMemoryManager(this);
		pStagingBufferManager = new VulkanStagingBufferManager(this);
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

	RHISwapchain* VulkanDevice::CreateSwapchain(RHISwapchainDescriptor* descriptor, WEngine::WArray<VkImage>& OutImages)
	{
		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.imageFormat = WEngine::ToVulkan(descriptor->format);
		swapchainCreateInfo.imageColorSpace = WEngine::ToVulkan(descriptor->colorSpace);
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

		return new VulkanSwapchain(this, static_cast<VulkanInstance*>(RHIContext::GetContext()->GetInstance()), &swapchainCreateInfo, OutImages);
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
		vkCreateEvent(pDevice, &eventCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pEvent);
		
		RHIEvent *event = (RHIEvent*)NormalAllocator::Get()->Allocate(sizeof(VulkanEvent));
		::new (event) VulkanEvent(pEvent);

		return event;
	}

	WVertexShaderRHIRef VulkanDevice::CreateVertexShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo ShaderModuleCreateInfo = {};
		ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ShaderModuleCreateInfo.codeSize = descriptor->codeSize;
		ShaderModuleCreateInfo.pCode = descriptor->pCode;

		return new VulkanVertexShader(this, &ShaderModuleCreateInfo);
	}

	WPixelShaderRHIRef VulkanDevice::CreatePixelShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo ShaderModuleCreateInfo = {};
		ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ShaderModuleCreateInfo.codeSize = descriptor->codeSize;
		ShaderModuleCreateInfo.pCode = descriptor->pCode;

		return new VulkanPixelShader(this, &ShaderModuleCreateInfo);
	}

	WGeometryShaderRHIRef VulkanDevice::CreateGeometryShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo ShaderModuleCreateInfo = {};
		ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ShaderModuleCreateInfo.codeSize = descriptor->codeSize;
		ShaderModuleCreateInfo.pCode = descriptor->pCode;

		return new VulkanGeometryShader(this, &ShaderModuleCreateInfo);
	}

	WComputeShaderRHIRef VulkanDevice::CreateComputeShader(RHIShaderDescriptor* descriptor)
	{
		VkShaderModuleCreateInfo ShaderModuleCreateInfo = {};
		ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ShaderModuleCreateInfo.codeSize = descriptor->codeSize;
		ShaderModuleCreateInfo.pCode = descriptor->pCode;

		return new VulkanComputeShader(this, &ShaderModuleCreateInfo);
	}

	WRenderPassRHIRef VulkanDevice::GetOrCreateRenderPass(RHIRenderPassDescriptor* descriptor)
	{
		uint32 RenderPassID = WEngine::MemCrc32(descriptor, sizeof(RHIRenderPassDescriptor));

		WRenderPassRHIRef RenderPass = RHIRenderPassManager::GetRenderPass(RenderPassID);
		if (RenderPass)
		{
			return RenderPass;
		}

		WEngine::WArray<VkAttachmentDescription> Attachments(descriptor->ColorAttachmentCount + (descriptor->bHasDepthStencilAttachment ? 1 : 0));
		WEngine::WArray<VkAttachmentReference> ColorReferences(descriptor->ColorAttachmentCount);
		for (uint32 ColorIndex = 0; ColorIndex < descriptor->ColorAttachmentCount; ++ColorIndex)
		{
			Attachments[ColorIndex].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			Attachments[ColorIndex].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			Attachments[ColorIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			Attachments[ColorIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;

			Attachments[ColorIndex].format = WEngine::ToVulkan(descriptor->ColorAttachmentDescriptors[ColorIndex].attachmentFormat);
			Attachments[ColorIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//WEngine::ToVulkan(descriptor->ColorAttachmentDescriptors[ColorIndex].attachmentLoadOP);
			Attachments[ColorIndex].storeOp = WEngine::ToVulkan(descriptor->ColorAttachmentDescriptors[ColorIndex].attachmentStoreOP);
			Attachments[ColorIndex].samples = WEngine::ToVulkan(descriptor->ColorAttachmentDescriptors[ColorIndex].sampleCount);

			ColorReferences[ColorIndex].attachment = ColorIndex;
			ColorReferences[ColorIndex].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		
		VkAttachmentReference DepthStencilReference;
		if (descriptor->bHasDepthStencilAttachment)
		{
			Attachments[descriptor->ColorAttachmentCount].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			Attachments[descriptor->ColorAttachmentCount].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			Attachments[descriptor->ColorAttachmentCount].format = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.attachmentFormat);
			Attachments[descriptor->ColorAttachmentCount].loadOp = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.attachmentLoadOP);
			Attachments[descriptor->ColorAttachmentCount].stencilLoadOp = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.stencilLoadOP);
			Attachments[descriptor->ColorAttachmentCount].samples = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.sampleCount);
			if (Attachments[descriptor->ColorAttachmentCount].samples == VK_SAMPLE_COUNT_1_BIT)
			{
				Attachments[descriptor->ColorAttachmentCount].storeOp = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.attachmentStoreOP);
				Attachments[descriptor->ColorAttachmentCount].stencilStoreOp = WEngine::ToVulkan(descriptor->DepthStencilAttachmentDescriptor.stencilStoreOP);
			}
			else
			{
				Attachments[descriptor->ColorAttachmentCount].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				Attachments[descriptor->ColorAttachmentCount].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			}

			DepthStencilReference.attachment = descriptor->ColorAttachmentCount;
			DepthStencilReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		
		WEngine::WArray<VkSubpassDescription> Subpasses(8);
		uint32 NumSubpass = 0;

		//main sub-pass
		{
			VkSubpassDescription& subpass = Subpasses[NumSubpass++];
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = ColorReferences.Size();
			subpass.pColorAttachments = ColorReferences.GetData();
			subpass.pDepthStencilAttachment = &DepthStencilReference;
		}

		VkRenderPassCreateInfo RenderPassCreateInfo = {};
		{
			RenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassCreateInfo.attachmentCount = Attachments.Size();
			RenderPassCreateInfo.pAttachments = Attachments.GetData();
			RenderPassCreateInfo.subpassCount = NumSubpass;
			RenderPassCreateInfo.pSubpasses = Subpasses.GetData();
			RenderPassCreateInfo.dependencyCount = 0;
			RenderPassCreateInfo.pDependencies = nullptr;
		}

		RenderPass = new VulkanRenderPass(this, &RenderPassCreateInfo);
		RHIRenderPassManager::AddRenderPass(RenderPassID, RenderPass);
		return RenderPass;
	}

	WFramebufferRHIRef VulkanDevice::GetOrCreateFramebuffer(RHIFramebufferDescriptor* descriptor, RHIRenderPass *RenderPass)
	{
		uint32 FramebufferID = WEngine::MemCrc32(descriptor, sizeof(RHIFramebufferDescriptor));
		WFramebufferRHIRef Framebuffer = RHIFramebufferManager::GetFramebuffer(FramebufferID);
		if (Framebuffer)
		{
			return Framebuffer;
		}

		WEngine::WArray<VkImageView> ImageViews(descriptor->AttachmentCount);
		for (uint32 Index = 0; Index < ImageViews.Size(); ++Index)
		{
			VulkanTextureBase *TextureBase = VulkanTextureBase::Cast(descriptor->Attachments[Index]);
			const VulkanSurface& Surface = TextureBase->GetSurface();

			VkImageViewCreateInfo ImageViewCreateInfo = {};
			{
				ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewCreateInfo.image = Surface.Image;
				ImageViewCreateInfo.format = Surface.format;
				ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
				ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
				ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
				ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
				ImageViewCreateInfo.viewType = Surface.ViewType;
				ImageViewCreateInfo.subresourceRange.aspectMask = Surface.Aspect;
				ImageViewCreateInfo.subresourceRange.layerCount = Surface.NumArray;
				ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewCreateInfo.subresourceRange.levelCount = Surface.NumMip;
				ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			}
			vkCreateImageView(pDevice, &ImageViewCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &ImageViews[Index]);
		}

		VkFramebufferCreateInfo FramebufferCreateInfo = {};
		{
			FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferCreateInfo.renderPass = static_cast<VulkanRenderPass*>(RenderPass)->GetHandle();
			FramebufferCreateInfo.attachmentCount = ImageViews.Size();
			FramebufferCreateInfo.pAttachments = ImageViews.GetData();
			FramebufferCreateInfo.width = descriptor->extent.width;
			FramebufferCreateInfo.height = descriptor->extent.height;
			FramebufferCreateInfo.layers = descriptor->extent.depth;
		}
		
		Framebuffer = new VulkanFramebuffer(this, &FramebufferCreateInfo);
		RHIFramebufferManager::AddFramebuffer(FramebufferID, Framebuffer);
		return Framebuffer;
	}

	RHIPipelineStateObject* VulkanDevice::GetOrCreateGraphicsPipelineState(RHIGraphicsPipelineStateDescriptor* descriptor)
	{
		uint32 PipelineID = WEngine::MemCrc32(descriptor, sizeof(RHIGraphicsPipelineStateDescriptor));
		VulkanGraphicsPipelineStateObject *Pipeline = VulkanPipelineStateManager::GetGraphicsPipelineState(PipelineID);
		if (Pipeline)
		{
			return Pipeline;
		}
		
		WEngine::WArray<VkPipelineShaderStageCreateInfo> ShaderStageCreateInfos;
		ShaderStageCreateInfos.Reserve(3);
		if (descriptor->Shaders[(uint8)EShaderStage::Vertex])
		{
			VkPipelineShaderStageCreateInfo ShaderStageCreateInfo = {};
			{
				ShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				ShaderStageCreateInfo.pName = "VSMain";
				ShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				ShaderStageCreateInfo.module = static_cast<VulkanVertexShader*>(static_cast<WVertexShader*>(descriptor->Shaders[(uint8)EShaderStage::Vertex])->GetRHI())->GetShaderModule();
			}
			ShaderStageCreateInfos.Push(ShaderStageCreateInfo);
		}
		if (descriptor->Shaders[(uint8)EShaderStage::Geometry])
		{
			VkPipelineShaderStageCreateInfo ShaderStageCreateInfo = {};
			{
				ShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				ShaderStageCreateInfo.pName = "GSMain";
				ShaderStageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				ShaderStageCreateInfo.module = static_cast<VulkanGeometryShader*>(static_cast<WGeometryShader*>(descriptor->Shaders[(uint8)EShaderStage::Vertex])->GetRHI())->GetShaderModule();
			}
			ShaderStageCreateInfos.Push(ShaderStageCreateInfo);
		}
		if (descriptor->Shaders[(uint8)EShaderStage::Pixel])
		{
			VkPipelineShaderStageCreateInfo ShaderStageCreateInfo = {};
			{
				ShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				ShaderStageCreateInfo.pName = "PSMain";
				ShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				ShaderStageCreateInfo.module = static_cast<VulkanPixelShader*>(static_cast<WPixelShader*>(descriptor->Shaders[(uint8)EShaderStage::Vertex])->GetRHI())->GetShaderModule();
			}
			ShaderStageCreateInfos.Push(ShaderStageCreateInfo);
		}

		WEngine::WArray<VkPipelineColorBlendAttachmentState> ColorBlendAttachments(descriptor->RenderTargetCount);
		for (uint32 AttachmentIndex = 0; AttachmentIndex < descriptor->RenderTargetCount; ++AttachmentIndex)
		{
			ColorBlendAttachments[AttachmentIndex] = (static_cast<VulkanBlendState*>(descriptor->BlendStates[AttachmentIndex])->ColorBlendAttachmentState);
		}
		VkPipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo = {};
		{
			ColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			ColorBlendStateCreateInfo.attachmentCount = ColorBlendAttachments.Size();
			ColorBlendStateCreateInfo.pAttachments = ColorBlendAttachments.GetData();
			ColorBlendStateCreateInfo.logicOpEnable = false;
		}

		VkDynamicState dynamicStates[3] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		{
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.dynamicStateCount = 3;
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
		}

		VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo = {};
		{
			GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			GraphicsPipelineCreateInfo.stageCount = ShaderStageCreateInfos.Size();
			GraphicsPipelineCreateInfo.pStages = ShaderStageCreateInfos.GetData();
			GraphicsPipelineCreateInfo.pVertexInputState = &static_cast<VulkanVertexInputState*>(descriptor->VertexInputState)->VertexInputStateCreateInfo;
			GraphicsPipelineCreateInfo.pInputAssemblyState = &static_cast<VulkanRasterizationState*>(descriptor->RasterizationState)->InputAssemblyStateCreateInfo;
			GraphicsPipelineCreateInfo.pViewportState = nullptr;
			GraphicsPipelineCreateInfo.pRasterizationState = &static_cast<VulkanRasterizationState*>(descriptor->RasterizationState)->RasterizationStateCreateInfo;
			GraphicsPipelineCreateInfo.pMultisampleState = &static_cast<VulkanMultiSampleState*>(descriptor->MultiSampleState)->MultiSampleStateCreateInfo;
			GraphicsPipelineCreateInfo.pDepthStencilState = &static_cast<VulkanDepthStencilState*>(descriptor->DepthStencilState)->DepthStencilStateCreateInfo;
			GraphicsPipelineCreateInfo.pColorBlendState = &ColorBlendStateCreateInfo;
			GraphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			GraphicsPipelineCreateInfo.layout = *static_cast<VulkanPipelineResourceLayout*>(descriptor->pipelineResourceLayout)->GetHandle();
			GraphicsPipelineCreateInfo.renderPass = static_cast<VulkanRenderPass*>(descriptor->RenderPass)->GetHandle();
			GraphicsPipelineCreateInfo.subpass = 0;
			GraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			//graphicsPipelineCreateInfo.basePipelineIndex = -1;
		}

		Pipeline = new VulkanGraphicsPipelineStateObject(this, &GraphicsPipelineCreateInfo);
		VulkanPipelineStateManager::AddGraphicsPipelineState(PipelineID, Pipeline);

		return Pipeline;
	}

	WTexture2DRHIRef VulkanDevice::CreateTexture2D(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo ImageCreateInfo = {};
		ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageCreateInfo.extent = { descriptor->width, descriptor->height, 1 };
		ImageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageCreateInfo.arrayLayers = 1;
		ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageCreateInfo.mipLevels = descriptor->mipCount;
		ImageCreateInfo.samples = WEngine::ToVulkan(descriptor->sampleCount);
		ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		ImageCreateInfo.usage = GetImageUsage(descriptor->Flag);

		ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		return new VulkanTexture2D(this, &ImageCreateInfo, descriptor->format, descriptor->Flag, descriptor->initialState, descriptor->clearValue, descriptor->sampleCount);
	}

	WTexture2DArrayRHIRef VulkanDevice::CreateTexture2DArray(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, 1 };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.arrayLayers = descriptor->layerCount;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = WEngine::ToVulkan(descriptor->sampleCount);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		imageCreateInfo.usage = GetImageUsage(descriptor->Flag);

		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		return new VulkanTexture2DArray(this, &imageCreateInfo, descriptor->format, descriptor->Flag, descriptor->initialState, descriptor->clearValue, descriptor->sampleCount);
	}

	WTexture3DRHIRef VulkanDevice::CreateTexture3D(RHITextureDescriptor* descriptor)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.extent = { descriptor->width, descriptor->height, descriptor->depth };
		imageCreateInfo.format = WEngine::ToVulkan(descriptor->format);
		imageCreateInfo.imageType = VK_IMAGE_TYPE_3D;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.mipLevels = descriptor->mipCount;
		imageCreateInfo.samples = WEngine::ToVulkan(descriptor->sampleCount);
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		imageCreateInfo.usage = GetImageUsage(descriptor->Flag);

		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		return new VulkanTexture3D(this, &imageCreateInfo, descriptor->format, descriptor->Flag, descriptor->initialState, descriptor->clearValue, descriptor->sampleCount);
	}

	WTextureViewRHIRef VulkanDevice::CreateTextureView(RHITextureViewDescriptor* descriptor, RHITexture* InTexture)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

			if(descriptor->dimension == Dimension::Texture2D)
				ImageViewCreateInfo.image = static_cast<VulkanTexture2D*>(InTexture)->GetHandle();
			else if(descriptor->dimension == Dimension::Texture2DARRAY)
				ImageViewCreateInfo.image = static_cast<VulkanTexture2DArray*>(InTexture)->GetHandle();
			else if(descriptor->dimension == Dimension::Texture3D)
				ImageViewCreateInfo.image = static_cast<VulkanTexture3D*>(InTexture)->GetHandle();

			ImageViewCreateInfo.format = WEngine::ToVulkan(descriptor->format);
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = descriptor->baseMipLevel;
			ImageViewCreateInfo.subresourceRange.levelCount = descriptor->mipCount;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = descriptor->baseArrayLayer;
			ImageViewCreateInfo.subresourceRange.layerCount = descriptor->arrayLayerCount;
			for (uint32 PlaneIndex = 0; PlaneIndex < descriptor->planeCount; ++PlaneIndex)
			{
				ImageViewCreateInfo.subresourceRange.aspectMask |= (VkImageAspectFlags)(1 << (PlaneIndex + descriptor->planeIndex));
			}
		}
		return new VulkanTextureView(this, &ImageViewCreateInfo);
	}

	WTextureViewRHIRef VulkanDevice::CreateTextureView(RHITextureViewDescriptor* descriptor, VkImage InImage)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ImageViewCreateInfo.image = InImage;
			ImageViewCreateInfo.format = WEngine::ToVulkan(descriptor->format);
			ImageViewCreateInfo.viewType = WEngine::ToVulkan(descriptor->dimension);
			ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
			ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
			ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
			ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			ImageViewCreateInfo.subresourceRange.baseMipLevel = descriptor->baseMipLevel;
			ImageViewCreateInfo.subresourceRange.levelCount = descriptor->mipCount;
			ImageViewCreateInfo.subresourceRange.baseArrayLayer = descriptor->baseArrayLayer;
			ImageViewCreateInfo.subresourceRange.layerCount = descriptor->arrayLayerCount;
			for (uint32 PlaneIndex = 0; PlaneIndex < descriptor->planeCount; ++PlaneIndex)
			{
				ImageViewCreateInfo.subresourceRange.aspectMask |= (VkImageAspectFlags)(1 << (PlaneIndex + descriptor->planeIndex));
			}
		}
		return new VulkanTextureView(this, &ImageViewCreateInfo);
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

	WVertexBufferRHIRef VulkanDevice::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanVertexBuffer(this, descriptor);
	}

	WDynamicVertexBufferRHIRef VulkanDevice::CreateDynamicVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanDynamicVertexBuffer(this, descriptor);
	}

	WIndexBufferRHIRef VulkanDevice::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanIndexBuffer(this, descriptor);
	}

	WUniformBufferRHIRef VulkanDevice::CreateUniformBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanUniformBuffer(this, descriptor);
	}

	WDynamicUniformBufferRHIRef VulkanDevice::CreateDynamicUniformBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanDynamicUniformBuffer(this, descriptor);
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
		WEngine::WArray<VkDescriptorPoolSize> DescriptorPoolSizes(descriptor->pGroupLayout->bindingCount);
		for (unsigned int i = 0; i < descriptor->pGroupLayout->bindingCount; ++i)
		{
			DescriptorPoolSizes[i].descriptorCount = descriptor->pGroupLayout->pBindingResources[i].count;
			DescriptorPoolSizes[i].type = WEngine::ToVulkan(descriptor->pGroupLayout->pBindingResources[i].type);
		}

		VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo = {};
		{
			DescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			DescriptorPoolCreateInfo.maxSets = descriptor->maxSetCount;
			DescriptorPoolCreateInfo.poolSizeCount = DescriptorPoolSizes.Size();
			DescriptorPoolCreateInfo.pPoolSizes = DescriptorPoolSizes.GetData();
		}

		return new VulkanGroupPool(this, &DescriptorPoolCreateInfo);
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

	WViewportRHIRef VulkanDevice::CreateViewport(RHIViewportDescriptor* descriptor)
	{
		return new VulkanViewport(this, descriptor);
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
				pDescriptorBufferInfos[i][j].buffer = static_cast<VulkanUniformBuffer*>((descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->pBuffer)->GetHandle();
				pDescriptorBufferInfos[i][j].offset = (descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->offset;
				pDescriptorBufferInfos[i][j].range = (descriptor->pBindingDescriptors[i].pBufferResourceInfo + j)->range;
			}

			::new (pWriteDescriptorSets + i) VkWriteDescriptorSet();
			pWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteDescriptorSets[i].descriptorCount = descriptor->pBindingResources[i].count;
			pWriteDescriptorSets[i].descriptorType = WEngine::ToVulkan(descriptor->pBindingResources[i].type);
			pWriteDescriptorSets[i].dstSet = static_cast<VulkanGroup*>(descriptor->pGroup)->GetHandle();
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
				pDescriptorImageInfos[i][j].imageView = info->pTextureView ? static_cast<VulkanTextureView*>(info->pTextureView)->GetHandle() : nullptr;
				pDescriptorImageInfos[i][j].sampler = info->pSampler ? *static_cast<VulkanSampler*>(info->pSampler)->GetHandle() : nullptr;
				pDescriptorImageInfos[i][j].imageLayout = WEngine::ToVulkan(info->layout);
			}

			::new (pWriteDescriptorSets + i) VkWriteDescriptorSet();
			pWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteDescriptorSets[i].descriptorCount = descriptor->pBindingResources[i].count;
			pWriteDescriptorSets[i].descriptorType = WEngine::ToVulkan(descriptor->pBindingResources[i].type);
			pWriteDescriptorSets[i].dstSet = static_cast<VulkanGroup*>(descriptor->pGroup)->GetHandle();
			pWriteDescriptorSets[i].dstBinding = descriptor->pBindingResources[i].bindingSlot;
			pWriteDescriptorSets[i].pImageInfo = pDescriptorImageInfos[i];
		}
		vkUpdateDescriptorSets(pDevice, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

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

	RHISemaphore* VulkanDevice::GetSemaphore()
	{
		return new VulkanSemaphore(this);
	}

	void VulkanDevice::WaitForFences(RHIFence* pFences, unsigned int count, bool waitForAll)
	{
		WEngine::WArray<VkFence> fences;
		fences.Reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.Push(static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		vkWaitForFences(pDevice, count, fences.GetData(), waitForAll, (std::numeric_limits<uint64_t>::max)());
	}

	void VulkanDevice::ResetFences(RHIFence* pFences, unsigned int count)
	{
		WEngine::WArray<VkFence> fences;
		fences.Reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.Push(static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		RE_ASSERT(vkResetFences(pDevice, count, fences.GetData()) == VK_SUCCESS, "Failed to Reset Fences.");
	}

	void VulkanDevice::SubmitCommandsAndFlushGPU()
	{
		VulkanCommandBufferManager *CmdBufferMgr = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager();
		if (CmdBufferMgr->HasPendingActiveCmdBuffer())
		{
			CmdBufferMgr->SubmitActiveCommandBuffer();
		}
		if (CmdBufferMgr->HasPendingImmediateCmdBuffer())
		{
			CmdBufferMgr->SubmitImmediateCommandBuffer();
		}
		CmdBufferMgr->PrepareForNewActiveCmdBuffer();
	}

	void VulkanDevice::PrepareForCPURead()
	{
		VulkanCommandBufferManager *CmdBufferMgr = static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager();
		VulkanCommandBuffer *ActiveCmdBuffer = CmdBufferMgr->GetActiveCommandBuffer();
		if (ActiveCmdBuffer && ActiveCmdBuffer->HasBegun())
		{
			CmdBufferMgr->SubmitActiveCommandBuffer();
			CmdBufferMgr->WaitForCommandBuffer(ActiveCmdBuffer);
		}
	}

	void VulkanDevice::Wait()
	{
		vkDeviceWaitIdle(pDevice);
		static_cast<VulkanContext*>(RHIContext::GetContext())->GetCmdBufferManager()->RefreshFenceState();
	}

	VkImageUsageFlags VulkanDevice::GetImageUsage(ETextureCreateFlags Flag)
	{
		VkImageUsageFlags Usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if ((Flag & ETextureCreateFlags::TextureCreate_Presentable) != ETextureCreateFlags::TextureCreate_None)
		{
			Usage |= VK_IMAGE_USAGE_STORAGE_BIT;
		}
		else if ((Flag & ETextureCreateFlags::TextureCreate_RenderTarget) != ETextureCreateFlags::TextureCreate_None)
		{
			if ((Flag & ETextureCreateFlags::TextureCreate_InputAttachmentReadable) != ETextureCreateFlags::TextureCreate_None)
			{
				Usage |= (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
			}
			else
			{
				Usage |= (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
			}
		}
		else if ((Flag & ETextureCreateFlags::TextureCreate_DepthStencil) != ETextureCreateFlags::TextureCreate_None)
		{
			if ((Flag & ETextureCreateFlags::TextureCreate_InputAttachmentReadable) != ETextureCreateFlags::TextureCreate_None)
			{
				Usage |= (VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
			}
			else
			{
				Usage |= (VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
			}
		}

		if ((Flag & ETextureCreateFlags::TextureCreate_UAV) != ETextureCreateFlags::TextureCreate_None)
		{
			Usage |= VK_IMAGE_USAGE_STORAGE_BIT;
		}

		return Usage;
	}

}
