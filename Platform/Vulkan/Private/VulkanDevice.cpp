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
		RE_ASSERT(vkCreateDevice(*pInGPU->GetHandle(), pInfo, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks(), &Device) == VK_SUCCESS, "Failed to Create Device.");

		pMemoryManager = new VulkanMemoryManager(this);
		pStagingBufferManager = new VulkanStagingBufferManager(this);
		pPipelineStateManager = new VulkanPipelineStateManager(this);
		pSamplerStateManager = new VulkanSamplerStateManager(this);
		pLayoutManager = new VulkanLayoutManager(this);

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
		swapchainCreateInfo.imageExtent = { descriptor->Extent.width, descriptor->Extent.height };
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

		VkEvent* pEvent = (VkEvent*)GetCPUAllocator()->Allocate(sizeof(VkEvent));
		vkCreateEvent(Device, &eventCreateInfo, static_cast<VulkanAllocator*>(GetCPUAllocator())->GetCallbacks(), pEvent);
		
		RHIEvent *event = (RHIEvent*)GetCPUAllocator()->Allocate(sizeof(VulkanEvent));
		::new (event) VulkanEvent(pEvent);

		return event;
	}

	WShaderRHIRef VulkanDevice::CreateShader(RHIShaderDescriptor* descriptor)
	{
		return new VulkanShader(this, descriptor);
	}

	WTextureRHIRef VulkanDevice::CreateTexture(const RHITextureDesc& InDesc)
	{
		return new VulkanTexture(this, InDesc);
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
		VkRect2D *pScissor = (VkRect2D*)GetCPUAllocator()->Allocate(sizeof(VkRect2D));
		pScissor->offset.x = descriptor->offsetX;
		pScissor->offset.y = descriptor->offsetY;
		pScissor->extent.width = descriptor->width;
		pScissor->extent.height = descriptor->height;

		VulkanScissor *scissor = (VulkanScissor*)GetCPUAllocator()->Allocate(sizeof(VulkanScissor));
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
