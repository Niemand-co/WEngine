#pragma once

class RHIGraphicsPipelineState;
class RHIComputePipelineState;
class RHIFence;
class RHISemaphore;
class RHIDevice;
class RHIRenderPass;
class RHIVertexBuffer;
class RHITexture;

namespace Vulkan
{

	class VulkanResource
	{
	public:

		void* operator new(size_t size)
		{
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

	};

	struct VulkanTextureView
	{
		VulkanTextureView()
			: ImageView(VK_NULL_HANDLE),
			  Image(VK_NULL_HANDLE)
		{
		}

		void Create(class VulkanDevice *Device, VkImage InImage, VkImageViewType ViewType, VkImageAspectFlags AspectFlags, EFormat Format, uint32 FirstMip, uint32 NumMips, uint32 FirstArray, uint32 NumArrays, VkImageUsageFlags UsageFlags = 0);

		void Destroy(class VulkanDevice *Device);

		VkImageView ImageView;
		VkImage Image;
	};

	template<typename RHIType>
	struct TVulkanResourceTraits
	{
	};

	template<>
	struct TVulkanResourceTraits<RHIAttachmentBlendState>
	{
		typedef class VulkanAttachmentBlendState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIVertexInputState>
	{
		typedef class VulkanVertexInputState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIDepthStencilState>
	{
		typedef class VulkanDepthStencilState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIRasterizationState>
	{
		typedef class VulkanRasterizationState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIMultiSampleState>
	{
		typedef class VulkanMultiSampleState type;
	};

	template<>
	struct TVulkanResourceTraits<RHISamplerState>
	{
		typedef class VulkanSamplerState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIShader>
	{
		typedef class VulkanShader type;
	};

	template<>
	struct TVulkanResourceTraits<RHITexture>
	{
		typedef class VulkanTexture type;
	};

	template<>
	struct TVulkanResourceTraits<RHIVertexBuffer>
	{
		typedef class VulkanVertexBuffer type;
	};

	template<>
	struct TVulkanResourceTraits<RHIUniformBuffer>
	{
		typedef class VulkanUniformBuffer type;
	};

	template<>
	struct TVulkanResourceTraits<RHIFence>
	{
		typedef class VulkanFence type;
	};

	template<>
	struct TVulkanResourceTraits<RHISemaphore>
	{
		typedef class VulkanSemaphore type;
	};

	template<>
	struct TVulkanResourceTraits<DynamicRHIContext>
	{
		typedef class VulkanDynamicContext type;
	};

	template<>
	struct TVulkanResourceTraits<StaticRHIContext>
	{
		typedef class VulkanStaticContext type;
	};

	template<>
	struct TVulkanResourceTraits<NormalAllocator>
	{
		typedef VulkanAllocator type;
	};

	template<>
	struct TVulkanResourceTraits<RHIGraphicsPipelineState>
	{
		typedef class VulkanGraphicsPipelineState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIComputePipelineState>
	{
		typedef class VulkanComputePipelineState type;
	};

	template<>
	struct TVulkanResourceTraits<RHIDevice>
	{
		typedef class VulkanDevice type;
	};

	template<>
	struct TVulkanResourceTraits<RHIRenderPass>
	{
		typedef class VulkanRenderPass type;
	};

	template<typename RHIType>
	static FORCEINLINE TVulkanResourceTraits<RHIType>::type* ResourceCast(RHIType* Resource)
	{
		return static_cast<typename TVulkanResourceTraits<RHIType>::type*>(Resource);
	}

	template<typename VulkanStructType>
	void ZeroVulkanStruct(VulkanStructType& Struct, int32 VulkanType)
	{
		Struct = {};
		(int32&)Struct.sType = VulkanType;
	}

}