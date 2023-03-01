#include "pch.h"
#include "Platform/Vulkan/Public/VulkanPipelineBarrier.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

namespace Vulkan
{

	static VkAccessFlags GetAccessForLayout(VkImageLayout Layout)
	{
		switch (Layout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
		case VK_IMAGE_LAYOUT_GENERAL:
			return 0;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			return VK_ACCESS_SHADER_READ_BIT;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			return VK_ACCESS_TRANSFER_READ_BIT;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			return VK_ACCESS_TRANSFER_WRITE_BIT;

		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
			return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
			return VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_TRANSFER_READ_BIT;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			return 0;

		default:
			return 0;
		}
	}

	static VkPipelineStageFlags GetPipelineStageForLayout(VkImageLayout Layout)
	{
		switch (Layout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
		case VK_IMAGE_LAYOUT_GENERAL:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			return VK_PIPELINE_STAGE_TRANSFER_BIT;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
		case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;	

		case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
			return VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;

		case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;

		default:
			return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		}
	}

	void GetVkStageAndAccessFlags(EAccess RHIAccess, bool IsDepthStencil, VkPipelineStageFlags& StageFlags, VkAccessFlags& AccessFlags, VkImageLayout& Layout)
	{
		switch (RHIAccess)
		{
		case EAccess::Unknown:
			StageFlags = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			AccessFlags = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
			Layout = VK_IMAGE_LAYOUT_UNDEFINED;
			return;
		case EAccess::CopySrc:
			StageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
			AccessFlags = VK_ACCESS_TRANSFER_READ_BIT;
			Layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			return;
		case EAccess::CopyDst:
			StageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
			AccessFlags = VK_ACCESS_TRANSFER_WRITE_BIT;
			Layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			return;
		case EAccess::SRVGraphics:
			StageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			AccessFlags = VK_ACCESS_SHADER_READ_BIT;
			Layout = IsDepthStencil ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			return;
		case EAccess::RTV:
			StageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			AccessFlags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			Layout = IsDepthStencil ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			return;
		case EAccess::HostReading:
			StageFlags = VK_PIPELINE_STAGE_HOST_BIT;
			AccessFlags = VK_ACCESS_HOST_READ_BIT;
			Layout = VK_IMAGE_LAYOUT_GENERAL;
			return;
		case EAccess::Present:
			StageFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			AccessFlags = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			Layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			return;
		case EAccess::Readable:
			StageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			AccessFlags = VK_ACCESS_MEMORY_READ_BIT;
			Layout = IsDepthStencil ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			return;
		case EAccess::Writable:
			StageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			AccessFlags = VK_ACCESS_MEMORY_WRITE_BIT;
			Layout = VK_IMAGE_LAYOUT_GENERAL;
			return;
		}
	}
	
	void VulkanPipelineBarrier::AddTransition(VkImage Image, VkImageSubresourceRange Range, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter)
	{
		VkImageMemoryBarrier Barrier;
		{
			Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			Barrier.image = Image;
			Barrier.oldLayout = LayoutBefore;
			Barrier.newLayout = LayoutAfter;
			Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.srcAccessMask = AccessBefore;
			Barrier.dstAccessMask = AccessAfter;
			Barrier.subresourceRange = Range;
		}

		SrcAccess |= AccessBefore;
		DstAccess |= AccessAfter;

		SrcStage |= GetPipelineStageForLayout(LayoutBefore);
		DstStage |= GetPipelineStageForLayout(LayoutAfter);

		ImageBarriers.Push(Barrier);
	}

	void VulkanPipelineBarrier::AddTransition(VkImage Image, VkImageAspectFlags ApsectMask, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter)
	{
		VkImageMemoryBarrier Barrier = {};
		{
			Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			Barrier.image = Image;
			Barrier.oldLayout = LayoutBefore;
			Barrier.newLayout = LayoutAfter;
			Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.srcAccessMask = AccessBefore;
			Barrier.dstAccessMask = AccessAfter;
			Barrier.subresourceRange = VkImageSubresourceRange(ApsectMask, 0, 1, 0, 1);
		}

		SrcAccess |= AccessBefore;
		DstAccess |= AccessAfter;

		SrcStage |= GetPipelineStageForLayout(LayoutBefore);
		DstStage |= GetPipelineStageForLayout(LayoutAfter);
		
		ImageBarriers.Push(Barrier);
	}

	void VulkanPipelineBarrier::AddTransition(VkImage Image, VkImageSubresourceRange Range, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter)
	{
		VkImageMemoryBarrier Barrier = {};
		{
			Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			Barrier.image = Image;
			Barrier.oldLayout = LayoutBefore;
			Barrier.newLayout = LayoutAfter;
			Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.srcAccessMask = GetAccessForLayout(LayoutBefore);
			Barrier.dstAccessMask = GetAccessForLayout(LayoutAfter);
			Barrier.subresourceRange = Range;
		}

		SrcAccess |= Barrier.srcAccessMask;
		DstAccess |= Barrier.dstAccessMask;

		SrcStage |= GetPipelineStageForLayout(LayoutBefore);
		DstStage |= GetPipelineStageForLayout(LayoutAfter);

		ImageBarriers.Push(Barrier);
	}

	void VulkanPipelineBarrier::AddTransition(VkImage Image, VkImageSubresourceRange Range, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout Layout)
	{
		VkImageMemoryBarrier Barrier = {};
		{
			Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			Barrier.image = Image;
			Barrier.oldLayout = Layout;
			Barrier.newLayout = Layout;
			Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.srcAccessMask = AccessBefore;
			Barrier.dstAccessMask = AccessAfter;
			Barrier.subresourceRange = Range;
		}

		SrcAccess |= AccessBefore;
		DstAccess |= AccessAfter;

		SrcAccess |= GetPipelineStageForLayout(Layout);
		DstStage |= GetPipelineStageForLayout(Layout);

		ImageBarriers.Push(Barrier);
	}

	void VulkanPipelineBarrier::AddTransition(VkBuffer Buffer, uint64 Size, uint64 Offset, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter)
	{
		VkBufferMemoryBarrier Barrier = {};
		{
			Barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
			Barrier.buffer = Buffer;
			Barrier.size = Size;
			Barrier.offset = Offset;
			Barrier.srcAccessMask = AccessBefore;
			Barrier.dstAccessMask = AccessAfter;
			Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		}

		SrcAccess |= AccessBefore;
		DstAccess |= AccessAfter;

		BufferBarriers.Push(Barrier);
	}

	void VulkanPipelineBarrier::AddTransition(VkAccessFlags AccessBefore, VkAccessFlags AccessAfger)
	{
		MemBarrier.srcAccessMask |= AccessBefore;
		MemBarrier.dstAccessMask |= AccessAfger;
	}

	void VulkanPipelineBarrier::Execute(VulkanCommandBuffer* CmdBuffer)
	{
		int32 NumMemoryCount = (MemBarrier.srcAccessMask != 0 || MemBarrier.dstAccessMask != 0) ? 1 : 0;
		if(NumMemoryCount == 0 && BufferBarriers.Size() == 0 && ImageBarriers.Size() == 0)
			return;
		vkCmdPipelineBarrier(CmdBuffer->GetHandle(), SrcStage, DstStage, 0, NumMemoryCount, &MemBarrier, BufferBarriers.Size(), BufferBarriers.GetData(), ImageBarriers.Size(), ImageBarriers.GetData());
	}

}