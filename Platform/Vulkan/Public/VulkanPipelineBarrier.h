#pragma once
#include "Render/Public/RenderDependencyGraphDefinitions.h"

namespace Vulkan
{
	void GetVkStageAndAccessFlags(EAccess RHIAccess, bool IsDepthStencil, VkPipelineStageFlags& StageFlags, VkAccessFlags& AccessFlags, VkImageLayout& Layout);

	class VulkanPipelineBarrier
	{
	public:

		VulkanPipelineBarrier()
		{
			MemBarrier = {};
			MemBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		}

		~VulkanPipelineBarrier() = default;

		void AddTransition(VkImage Image, VkImageSubresourceRange Range, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter);
		void AddTransition(VkImage Image, VkImageAspectFlags ApsectMask, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter);
		void AddTransition(VkImage Image, VkImageSubresourceRange Range, VkImageLayout LayoutBefore, VkImageLayout LayoutAfter);
		void AddTransition(VkImage Image, VkImageSubresourceRange Range, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter, VkImageLayout Layout);

		void AddTransition(VkBuffer Buffer, uint64 Size, uint64 Offset, VkAccessFlags AccessBefore, VkAccessFlags AccessAfter);

		void AddTransition(VkAccessFlags AccessBefore, VkAccessFlags AccessAfger);

		void Execute(class VulkanCommandBuffer *CmdBuffer);

		static VkImageSubresourceRange GetTextureSubresourceRange(VkImageAspectFlags AspectMask, uint32 BaseMipLevel, uint32 MipLevelCount, uint32 BaseArrayLayer, uint32 ArrayLayerCount)
		{
			return VkImageSubresourceRange(AspectMask, BaseMipLevel, MipLevelCount, BaseArrayLayer, ArrayLayerCount);
		}

	private:

		VkPipelineStageFlags SrcStage = 0, DstStage = 0;

		VkAccessFlags SrcAccess = 0, DstAccess = 0;

		VkMemoryBarrier MemBarrier;

		WEngine::WArray<VkImageMemoryBarrier> ImageBarriers;

		WEngine::WArray<VkBufferMemoryBarrier> BufferBarriers;

	};

}