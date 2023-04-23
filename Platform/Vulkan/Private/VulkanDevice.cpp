#include "pch.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Platform/Vulkan/Allocator/Public/VulkanMemoryManager.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Utils/Public/Window.h"
#include "Render/Mesh/Public/Vertex.h"
#include "RHI/Public/RHIBuffer.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Public/Shader.h"
#include "Render/Public/MeshPassProcessor.h"

namespace Vulkan
{

	VulkanDevice::VulkanDevice(VulkanGPU *pInGPU, VkDeviceCreateInfo* pInfo, WEngine::WArray<QueueStack>& InQueueStack)
		: pGPU(pInGPU), m_queues(InQueueStack)
	{
		RE_ASSERT(vkCreateDevice(*pInGPU->GetHandle(), pInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &Device) == VK_SUCCESS, "Failed to Create Device.");

		pMemoryManager = new VulkanMemoryManager(this);
		pStagingBufferManager = new VulkanStagingBufferManager(this);

		vkGetPhysicalDeviceProperties(*pGPU->GetHandle(), &GPUProps);
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

		return new VulkanSwapchain(this, static_cast<VulkanInstance*>(GetDynamicRHI()->GetInstance()), &swapchainCreateInfo, OutImages);
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
		vkCreateEvent(Device, &eventCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pEvent);
		
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
		
		VkAttachmentReference DepthStencilReference = {};
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
			subpass.pDepthStencilAttachment = descriptor->bHasDepthStencilAttachment ? &DepthStencilReference : nullptr;
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
			VulkanTexture *TextureRHI = VulkanTextureBase::Cast(descriptor->Attachments[Index]);

			VkImageViewCreateInfo ImageViewCreateInfo = {};
			{
				ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewCreateInfo.image = TextureRHI->Image;
				ImageViewCreateInfo.format = TextureRHI->Format;
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
			vkCreateImageView(Device, &ImageViewCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &ImageViews[Index]);
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
		auto IsTextureResource = [](EUniformBaseType Type)->bool { return Type == EUniformBaseType::UB_TEXTURE ||
																		  Type == EUniformBaseType::UB_SRV ||
																		  Type == EUniformBaseType::UB_UAV; };

		WMeshDrawShaderBindings* ShaderBindings = descriptor->ShaderBindings;
		WEngine::WArray<uint32> HashCodes(MaxGraphicsPipelineShaderNum);
		WEngine::WArray<VkDescriptorSetLayout> DescriptorSetLayouts;
		for (uint32 ShaderStage = 0; ShaderStage < MaxGraphicsPipelineShaderNum; ++ShaderStage)
		{
			const WMeshDrawShaderBindings& Bindings = ShaderBindings[ShaderStage];

			uint32 LayoutID = Bindings.GetHashCode();
			HashCodes[ShaderStage] = LayoutID;
			VulkanDescriptorSetLayout *DescriptorSetLayout = VulkanDescriptorSetLayoutManager::GetDescriptorSetLayout(LayoutID);
			if (DescriptorSetLayout)
			{
				DescriptorSetLayouts.Push(DescriptorSetLayout->GetHandle());
				continue;
			}

			VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo = {};
			{
				DescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			}
			WEngine::WArray<VkDescriptorSetLayoutBinding> LayoutBindings;
			Bindings.EnumerateBindings([IsTextureResource, &DescriptorSetLayoutCreateInfo, &LayoutBindings, ShaderStage](const ShaderBindingSlot& Binding) -> void
			{
				DescriptorSetLayoutCreateInfo.bindingCount++;
				if (IsTextureResource(Binding.Type))
				{
					LayoutBindings.Push(VkDescriptorSetLayoutBinding(Binding.Slot, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, Binding.Count, WEngine::ToVulkan(EShaderStage(ShaderStage))));
				}
				else
				{
					LayoutBindings.Push(VkDescriptorSetLayoutBinding(Binding.Slot, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, Binding.Count, WEngine::ToVulkan(EShaderStage(ShaderStage))));
				}
			});

			if (LayoutBindings.Size() == 0)
			{
				continue;
			}

			{
				DescriptorSetLayoutCreateInfo.bindingCount = LayoutBindings.Size();
				DescriptorSetLayoutCreateInfo.pBindings = LayoutBindings.GetData();
			}

			DescriptorSetLayout = new VulkanDescriptorSetLayout(this, &DescriptorSetLayoutCreateInfo);
			DescriptorSetLayouts.Push(DescriptorSetLayout->GetHandle());
			VulkanDescriptorSetLayoutManager::AddDescriptorLayout(LayoutID, DescriptorSetLayout);
		}

		uint32 PipelineLayoutID = WEngine::MemCrc32(HashCodes.GetData(), HashCodes.Size() * 4);
		descriptor->Finalize(PipelineLayoutID);
		uint32 PipelineID = descriptor->GetHashCode();
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
				ShaderStageCreateInfo.module = static_cast<VulkanVertexShader*>(descriptor->Shaders[(uint8)EShaderStage::Vertex]->GetVertexShader())->GetShaderModule();
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
				ShaderStageCreateInfo.module = static_cast<VulkanGeometryShader*>(descriptor->Shaders[(uint8)EShaderStage::Geometry]->GetGeometryShader())->GetShaderModule();
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
				ShaderStageCreateInfo.module = static_cast<VulkanPixelShader*>(descriptor->Shaders[(uint8)EShaderStage::Pixel]->GetPixelShader())->GetShaderModule();
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
			ColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
			ColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		}

		VkDynamicState dynamicStates[3] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		{
			dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCreateInfo.dynamicStateCount = 3;
			dynamicStateCreateInfo.pDynamicStates = dynamicStates;
		}

		VkPipelineLayout PipelineLayout;
		VkPipelineLayoutCreateInfo PipelineCreateInfo = {};
		{
			PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			PipelineCreateInfo.pushConstantRangeCount = 0;
			PipelineCreateInfo.setLayoutCount = DescriptorSetLayouts.Size();
			PipelineCreateInfo.pSetLayouts = DescriptorSetLayouts.GetData();
		}
		vkCreatePipelineLayout(Device, &PipelineCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), &PipelineLayout);

		VkViewport viewport = {0, 0, 1920, 1080, 0.0f, 1.0f};
		VkRect2D scissor = {0, 0, 1920, 1080};
		VkPipelineViewportStateCreateInfo PipelineViewportStateCreateInfo = {};
		{
			PipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			PipelineViewportStateCreateInfo.viewportCount = 1;
			PipelineViewportStateCreateInfo.pViewports = &viewport;
			PipelineViewportStateCreateInfo.scissorCount = 1;
			PipelineViewportStateCreateInfo.pScissors = &scissor;
		}

		VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo = {};
		{
			GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			GraphicsPipelineCreateInfo.stageCount = ShaderStageCreateInfos.Size();
			GraphicsPipelineCreateInfo.pStages = ShaderStageCreateInfos.GetData();
			GraphicsPipelineCreateInfo.pVertexInputState = &static_cast<VulkanVertexInputState*>(descriptor->VertexInputState)->VertexInputStateCreateInfo;
			GraphicsPipelineCreateInfo.pInputAssemblyState = &static_cast<VulkanRasterizationState*>(descriptor->RasterizationState)->InputAssemblyStateCreateInfo;
			GraphicsPipelineCreateInfo.pViewportState = &PipelineViewportStateCreateInfo;
			GraphicsPipelineCreateInfo.pRasterizationState = &static_cast<VulkanRasterizationState*>(descriptor->RasterizationState)->RasterizationStateCreateInfo;
			GraphicsPipelineCreateInfo.pMultisampleState = &static_cast<VulkanMultiSampleState*>(descriptor->MultiSampleState)->MultiSampleStateCreateInfo;
			GraphicsPipelineCreateInfo.pDepthStencilState = &static_cast<VulkanDepthStencilState*>(descriptor->DepthStencilState)->DepthStencilStateCreateInfo;
			GraphicsPipelineCreateInfo.pColorBlendState = &ColorBlendStateCreateInfo;
			GraphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
			GraphicsPipelineCreateInfo.layout = PipelineLayout;
			GraphicsPipelineCreateInfo.renderPass = static_cast<VulkanRenderPass*>(descriptor->RenderPass)->GetHandle();
			GraphicsPipelineCreateInfo.subpass = 0;
			GraphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			//graphicsPipelineCreateInfo.basePipelineIndex = -1;
		}

		Pipeline = new VulkanGraphicsPipelineStateObject(this, &GraphicsPipelineCreateInfo);
		VulkanPipelineStateManager::AddGraphicsPipelineState(PipelineID, Pipeline);

		return Pipeline;
	}

	WTextureRHIRef VulkanDevice::CreateTexture(const RHITextureDesc& InDesc)
	{
		return new VulkanTexture(this, InDesc);
	}

	WTextureViewRHIRef VulkanDevice::CreateTextureView(RHITextureViewDescriptor* descriptor, RHITexture* InTexture)
	{
		VkImageViewCreateInfo ImageViewCreateInfo = {};
		{
			ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

			if(descriptor->Dimension == EDimension::Texture2D)
				ImageViewCreateInfo.image = static_cast<VulkanTexture*>(InTexture)->GetHandle();
			else if(descriptor->Dimension == EDimension::Texture2DArray)
				ImageViewCreateInfo.image = static_cast<VulkanTexture*>(InTexture)->GetHandle();
			else if(descriptor->Dimension == EDimension::Texture3D)
				ImageViewCreateInfo.image = static_cast<VulkanTexture*>(InTexture)->GetHandle();

			ImageViewCreateInfo.format = WEngine::ToVulkan(descriptor->Format);
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
			ImageViewCreateInfo.format = WEngine::ToVulkan(descriptor->Format);
			ImageViewCreateInfo.viewType = WEngine::ToVulkan(descriptor->Dimension);
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
		RE_ASSERT(vkCreateSampler(Device, &samplerCreateInfo, static_cast<VulkanAllocator*>(NormalAllocator::Get())->GetCallbacks(), pSampler) == VK_SUCCESS, "Failed to Create Sampler.");
		
		VulkanSampler *sampler = (VulkanSampler*)NormalAllocator::Get()->Allocate(sizeof(VulkanSampler));
		::new (sampler) VulkanSampler(pSampler);

		return sampler;
	}

	WVertexBufferRHIRef VulkanDevice::CreateVertexBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanVertexBuffer(this, descriptor);
	}

	WIndexBufferRHIRef VulkanDevice::CreateIndexBuffer(RHIBufferDescriptor* descriptor)
	{
		return new VulkanIndexBuffer(this, descriptor);
	}

	WUniformBufferRHIRef VulkanDevice::CreateUniformBuffer(uint8* InContents, const ShaderParametersLayout* InLayout, EBufferUsageFlags InUsage)
	{
		return new VulkanUniformBuffer(this, InContents, InLayout, InUsage);
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
		vkUpdateDescriptorSets(Device, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

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
		vkUpdateDescriptorSets(Device, descriptor->bindingCount, pWriteDescriptorSets, 0, nullptr);

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

		vkWaitForFences(Device, count, fences.GetData(), waitForAll, (std::numeric_limits<uint64_t>::max)());
	}

	void VulkanDevice::ResetFences(RHIFence* pFences, unsigned int count)
	{
		WEngine::WArray<VkFence> fences;
		fences.Reserve(count);
		for (unsigned int i = 0; i < count; ++i)
		{
			fences.Push(static_cast<VulkanFence*>(pFences + i)->GetHandle());
		}

		RE_ASSERT(vkResetFences(Device, count, fences.GetData()) == VK_SUCCESS, "Failed to Reset Fences.");
	}

	void VulkanDevice::SubmitCommandsAndFlushGPU()
	{
		VulkanCommandBufferManager *CmdBufferMgr = static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager();
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
		VulkanCommandBufferManager *CmdBufferMgr = static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager();
		VulkanCommandBuffer *ActiveCmdBuffer = CmdBufferMgr->GetActiveCommandBuffer();
		if (ActiveCmdBuffer && ActiveCmdBuffer->HasBegun())
		{
			CmdBufferMgr->SubmitActiveCommandBuffer();
			CmdBufferMgr->WaitForCommandBuffer(ActiveCmdBuffer);
		}
	}

	void VulkanDevice::Wait()
	{
		vkDeviceWaitIdle(Device);
		static_cast<VulkanDynamicContext*>(GetDynamicRHI())->GetCmdBufferManager()->RefreshFenceState();
	}

}
