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
		pPipelineStateManager = new VulkanPipelineStateManager(this);

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
			VulkanTexture *TextureRHI = static_cast<VulkanTexture*>(descriptor->Attachments[Index]);

			VkImageViewCreateInfo ImageViewCreateInfo = {};
			{
				ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewCreateInfo.image = TextureRHI->GetHandle();
				ImageViewCreateInfo.format = WEngine::ToVulkan(TextureRHI->GetDesc().Format);
				ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
				ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
				ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
				ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
				ImageViewCreateInfo.viewType = TextureRHI->GetViewType(TextureRHI->GetDesc().Dimension);
				ImageViewCreateInfo.subresourceRange.aspectMask = TextureRHI->GetAspect(TextureRHI->GetDesc().Format);
				ImageViewCreateInfo.subresourceRange.layerCount = TextureRHI->GetDesc().ArraySize;
				ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewCreateInfo.subresourceRange.levelCount = TextureRHI->GetDesc().NumMips;
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

	}

	WTextureRHIRef VulkanDevice::CreateTexture(const RHITextureDesc& InDesc)
	{
		return new VulkanTexture(this, InDesc);
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

	WUniformBufferRHIRef VulkanDevice::CreateUniformBuffer(uint8* InContents, const ShaderParametersLayout* InLayout, EUniformBufferUsage InUsage)
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
