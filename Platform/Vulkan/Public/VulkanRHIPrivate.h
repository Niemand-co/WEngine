#pragma once

#define VULKAN_API
#ifdef VULKAN_API
#include "Render/Public/RenderCore.h"
#include "Utils/Container/Public/WArray.h"
#include "RHI/Public/RHICore.h"
#include "Utils/Container/Public/WHashMap.h"

enum class EBufferUsageFlags : uint32
{
	BF_None            = 0x0,
	BF_TransferSrc     = 0x00000001,
	BF_TransferDst     = 0x00000002,
	BF_UniformTexel    = 0x00000004,
	BF_StorageTexel    = 0x00000008,
	BF_UniformBuffer   = 0x00000010,
	BF_SingleFrame     = 0x00000020,
	BF_StorageBuffer   = 0x00000040,
	BF_IndexBuffer     = 0x00000080,
	BF_VertexBuffer    = 0x00000100,
	BF_IndirectBuffer  = 0x00000200,
	BF_SRV             = 0x00000400,
	BF_UAV             = 0x00000800,
	BF_Dynamic         = 0x00001000,
	BF_CPUAccessable   = 0x00002000,
};
ENUM_CLASS_FLAGS(EBufferUsageFlags)

enum class EImageUsageFlags : uint32
{
	IM_CopySrc                = 0x00000001,
	IM_CopyDst                = 0x00000002,
	IM_Sampled                = 0x00000004,
	IM_Storage                = 0x00000008,
	IM_ColorAttachment        = 0x00000010,
	IM_DepthStencilAttachment = 0x00000020,
	IM_TransientAttachment    = 0x00000040,
	IM_InputAttachment        = 0x00000080,
};

template<typename FlagBits>
bool VkEnumHasFlags(VkFlags Flags, FlagBits Bits)
{
	return (Flags & Bits) == Bits;
}

constexpr const VkComponentMapping ComponentMappingRGBA = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
constexpr const VkComponentMapping ComponentMappingRGB1 = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ONE };
constexpr const VkComponentMapping ComponentMappingRG01 = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE };
constexpr const VkComponentMapping ComponentMappingR001 = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE };
constexpr const VkComponentMapping ComponentMappingRIII = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
constexpr const VkComponentMapping ComponentMapping000R = { VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_R };
constexpr const VkComponentMapping ComponentMappingR000 = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO };
constexpr const VkComponentMapping ComponentMappingRR01 = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE };

static VkComponentMapping FormatComponentMapping(EFormat Format)
{
	switch (Format)
	{
		case EFormat::R8_SInt:
		case EFormat::R8_SNorm:
		case EFormat::R8_UInt:
		case EFormat::R8_UNorm:
		case EFormat::R16_UInt:
		case EFormat::R16_SInt:
		case EFormat::R16_SFloat:
		case EFormat::R32_UInt:
		case EFormat::R32_SInt:
		case EFormat::R32_SFloat:
			return ComponentMappingR001;
		case EFormat::D16_Unorm:
			return ComponentMappingRIII;
		case EFormat::D32_SFloat:
			return ComponentMappingR000;
		case EFormat::D16_UNORM_S8_UINT:
		case EFormat::D24_UNORM_S8_UINT:
		case EFormat::D32_SFLOAT_S8_UINT: 
			return ComponentMappingR000;
		case EFormat::R16G16_UInt:
		case EFormat::R16G16_SInt:
		case EFormat::R16G16_SFloat:
			return ComponentMappingRG01;
		default:
			return ComponentMappingRGBA;
	}

}
class RHIGraphicsPipelineStateInitializer;
class RHIRenderPassDescriptor;
class RHIFramebufferDescriptor;

namespace Vulkan
{

	class VulkanRenderTargetLayout
	{
	public:

		VulkanRenderTargetLayout(const RHIGraphicsPipelineStateInitializer& Initializer);

		VulkanRenderTargetLayout(const RHIRenderPassDescriptor* Descriptor);

	public:

		VkAttachmentDescription Attachments[MaxSimultaneousRenderTargets * 2 + 2];

		VkAttachmentReference ColorReferences[MaxSimultaneousRenderTargets];
		VkAttachmentReference DepthStencilReference;
		VkAttachmentReference InputReferences[MaxSimultaneousRenderTargets + 1];
		VkAttachmentReference ResolveReferences[MaxSimultaneousRenderTargets];

		uint8 NumAttachments;
		uint8 NumColorAttachments;
		uint8 NumInputAttachments;
		uint8 bHasDepthStencilAttachment : 1;
		uint8 bHasResolveAttachments : 1;
		uint8 NumUsedClearValues;

		uint32 Hash;

	};

	class VulkanLayoutManager
	{
	public:

		VulkanLayoutManager(class VulkanDevice *pInDevice)
			: pDevice(pInDevice),
			  CurrentRenderPass(nullptr),
			  CurrentFramebuffer(nullptr)
		{
		}

		class VulkanRenderPass* GetOrCreateRenderPass(const VulkanRenderTargetLayout& RTLayout);

		class VulkanFramebuffer* GetOrCreateFramebuffer(const RHIFramebufferDescriptor* RTInfo, const VulkanRenderTargetLayout& RTLayout, VulkanRenderPass* RenderPass);

	public:

		VulkanRenderPass *CurrentRenderPass;

		VulkanFramebuffer *CurrentFramebuffer;

	private:

		WCriticalSection RenderPassLock;

		WCriticalSection FramebufferLock;

		class VulkanDevice *pDevice;

		WEngine::WHashMap<uint32, VulkanRenderPass*> RenderPasses;

		struct FramebufferList
		{
			WEngine::WArray<VulkanFramebuffer*> Framebuffer;
		};
		WEngine::WHashMap<uint32, FramebufferList*> Framebuffers;
		
	};

}

#endif