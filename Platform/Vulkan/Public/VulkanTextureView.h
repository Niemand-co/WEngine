#pragma once
#include "RHI/Public/RHITextureView.h"

class RHITextureViewDescriptor;
class VulkanFramebuffer;

namespace Vulkan
{

	class VulkanTextureView : public RHITextureView
	{
	public:

		VulkanTextureView(VkImageView *imageView, RHITextureViewDescriptor* descriptor);

		virtual ~VulkanTextureView();

		VkImageView* GetHandle();

		friend VulkanFramebuffer;

	private:

		VkImageView *m_imageView;

		Format m_format;

		unsigned int m_mipCount;

		unsigned int m_baseMipLevel;

		unsigned int m_arrayLayerCount;

		unsigned int m_baseArrayLayer;

		Dimension m_dimension;

	};

}