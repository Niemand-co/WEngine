#pragma once

namespace WEngine
{
	static VkFormat ToVulkan(Format format)
	{
		switch (format)
		{
		case Format::R8_SInt:
			return VK_FORMAT_R8_SINT;
		case Format::R8_UInt:
			return VK_FORMAT_R8_UINT;
		case Format::R8_UNorm:
			return VK_FORMAT_R8_UNORM;
		case Format::R8_SNorm:
			return VK_FORMAT_R8_SNORM;
		case Format::R8G8_SFloat:
			return VK_FORMAT_R8G8_SINT;
		case Format::R8G8_SInt:
			return VK_FORMAT_R8G8_SINT;
		case Format::R8G8_UInt:
			return VK_FORMAT_R8G8_UINT;
		case Format::R8G8_UNorm:
			return VK_FORMAT_R8G8_UNORM;
		case Format::R8G8_SNorm:
			return VK_FORMAT_R8G8_SNORM;
		case Format::R8G8B8_SInt:
			return VK_FORMAT_R8G8B8_SINT;
		case Format::R8G8B8_UInt:
			return VK_FORMAT_R8G8B8_UINT;
		case Format::R8G8B8_UNorm:
			return VK_FORMAT_R8G8B8_UNORM;
		case Format::R8G8B8_SNorm:
			return VK_FORMAT_R8G8B8_SNORM;
		case Format::A8R8G8B8_SInt:
			return VK_FORMAT_R8G8B8_SINT;
		case Format::A8R8G8B8_UInt:
			return VK_FORMAT_R8G8B8A8_UINT;
		case Format::A8R8G8B8_UNorm:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case Format::A8R8G8B8_SNorm:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case Format::R16_SFloat:
			return VK_FORMAT_R16_SFLOAT;
		case Format::R16_SInt:
			return VK_FORMAT_R16_SINT;
		case Format::R16_UInt:
			return VK_FORMAT_R16_UINT;
		case Format::R16G16_SFloat:
			return VK_FORMAT_R16G16_SFLOAT;
		case Format::R16G16_SInt:
			return VK_FORMAT_R16G16_SINT;
		case Format::R16G16_UInt:
			return VK_FORMAT_R16G16_UINT;
		case Format::R16G16B16_SFloat:
			return VK_FORMAT_R16G16B16_SFLOAT;
		case Format::R16G16B16_SInt:
			return VK_FORMAT_R16G16B16_SINT;
		case Format::R16G16B16_UInt:
			return VK_FORMAT_R16G16B16_UINT;
		case Format::A16R16G16B16_SFloat:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case Format::A16R16G16B16_SInt:
			return VK_FORMAT_R16G16B16A16_SINT;
		case Format::A16R16G16B16_UInt:
			return VK_FORMAT_R16G16B16A16_UINT;
		case Format::R32_SFloat:
			return VK_FORMAT_R32_SFLOAT;
		case Format::R32_SInt:
			return VK_FORMAT_R32_SINT;
		case Format::R32_UInt:
			return VK_FORMAT_R32_UINT;
		case Format::R32G32_SFloat:
			return VK_FORMAT_R32G32_SFLOAT;
		case Format::R32G32_SInt:
			return VK_FORMAT_R32G32_SINT;
		case Format::R32G32_UInt:
			return VK_FORMAT_R32G32_UINT;
		case Format::R32G32B32_SFloat:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case Format::R32G32B32_SInt:
			return VK_FORMAT_R32G32B32_SINT;
		case Format::R32G32B32_UInt:
			return VK_FORMAT_R32G32B32_UINT;
		case Format::A32R32G32B32_SFloat:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case Format::A32R32G32B32_SInt:
			return VK_FORMAT_R32G32B32A32_SINT;
		case Format::A32R32G32B32_UInt:
			return VK_FORMAT_R32G32B32A32_UINT;
		case Format::D32_SFloat:
			return VK_FORMAT_D32_SFLOAT;
		case Format::D16_Unorm:
			return VK_FORMAT_D16_UNORM;
		case Format::B8G8R8A8_UNorm:
			return VK_FORMAT_B8G8R8A8_UNORM;
		default:
			RE_ASSERT(false, "Error Type Format.");
		}
		return VK_FORMAT_MAX_ENUM;
	}

	static VkColorSpaceKHR ToVulkan(ColorSpace colorSpace)
	{
		switch (colorSpace)
		{
		case ColorSpace::SRGB_Linear:
			return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		default:
			RE_ASSERT(false, "Error Type Colorspace.");
		}
		return VK_COLOR_SPACE_MAX_ENUM_KHR;
	}

	static VkPresentModeKHR ToVulkan(PresentMode presentMode)
	{
		switch (presentMode)
		{
		case PresentMode::Immediate:
			return VK_PRESENT_MODE_IMMEDIATE_KHR;
		case PresentMode::FIFO:
			return VK_PRESENT_MODE_FIFO_KHR;
		case PresentMode::FIFO_Relaxed:
			return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		case PresentMode::Mailbox:
			return VK_PRESENT_MODE_MAILBOX_KHR;
		default:
			RE_ASSERT(false, "Error Type PresentMode.");
		}
		return VK_PRESENT_MODE_MAX_ENUM_KHR;
	}

	static VkImageViewType ToVulkan(Dimension dimension)
	{
		switch (dimension)
		{
		case Dimension::Texture1D:
			return VK_IMAGE_VIEW_TYPE_1D;
		case Dimension::Texture2D:
			return VK_IMAGE_VIEW_TYPE_2D;
		case Dimension::Texture3D:
			return VK_IMAGE_VIEW_TYPE_3D;
		case Dimension::TextureCUBE:
			return VK_IMAGE_VIEW_TYPE_CUBE;
		case Dimension::Texture1DARRAY:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case Dimension::Texture2DARRAY:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case Dimension::TextureCUBEARRAY:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		default:
			RE_ASSERT(false, "Error Type View Type.");
		}
		return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
	}

	static VkCompareOp ToVulkan(DepthCompareOP op)
	{
		switch (op)
		{
		case DepthCompareOP::Greater:
			return VK_COMPARE_OP_GREATER;
		case DepthCompareOP::GE:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case DepthCompareOP::Less:
			return VK_COMPARE_OP_LESS;
		case DepthCompareOP::LE:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		default:
			break;
		}
		return VK_COMPARE_OP_MAX_ENUM;
	}

	static VkBlendFactor ToVulkan(BlendFactor factor)
	{
		switch (factor)
		{
		case BlendFactor::FactorOne:
			return VK_BLEND_FACTOR_ONE;
		case BlendFactor::FactorZero:
			return VK_BLEND_FACTOR_ZERO;
		case BlendFactor::FactorSrcAlpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;
		case BlendFactor::FactorDstAlpha:
			return VK_BLEND_FACTOR_DST_ALPHA;
		case BlendFactor::FactorOneMinusSrcAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::FactorOneMinusDstAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case BlendFactor::FactorConstantAlpha:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;
		case BlendFactor::FactorOneMinusConstantAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		default:
			RE_ASSERT(false, "Error Type Blend Factor.");
		}
		return VK_BLEND_FACTOR_MAX_ENUM;
	}

	static VkBlendOp ToVulkan(BlendOP op)
	{
		switch (op)
		{
		case BlendOP::BlendAdd:
			return VK_BLEND_OP_ADD;
		case BlendOP::BlendSubtract:
			return VK_BLEND_OP_SUBTRACT;
		case BlendOP::BlendReverse_Subtract:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
		case BlendOP::BlendMin:
			return VK_BLEND_OP_MIN;
		case BlendOP::BlendMax:
			return VK_BLEND_OP_MAX;
		default:
			RE_ASSERT(false, "Error Type Blend Operation.");
		}
		return VK_BLEND_OP_MAX_ENUM;
	}

	static VkAttachmentLoadOp ToVulkan(AttachmentLoadOP op)
	{
		switch (op)
		{
		case AttachmentLoadOP::Load:
			return VK_ATTACHMENT_LOAD_OP_LOAD;
		case AttachmentLoadOP::DontCare:
			return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		case AttachmentLoadOP::Clear:
			return VK_ATTACHMENT_LOAD_OP_CLEAR;
		default:
			RE_ASSERT(false, "Error Type Attachment Load Operation.");
		}
		return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
	}

	static VkAttachmentStoreOp ToVulkan(AttachmentStoreOP op)
	{
		switch (op)
		{
		case AttachmentStoreOP::Store:
			return VK_ATTACHMENT_STORE_OP_STORE;
		case AttachmentStoreOP::DontCare:
			return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		default:
			RE_ASSERT(false, "Error Type Attachment Store Operation.");
		}
		return VK_ATTACHMENT_STORE_OP_MAX_ENUM;
	}

	static VkSampleCountFlagBits ToVulkan(unsigned int sampleCount)
	{
		switch (sampleCount)
		{
		case 1:
			return VK_SAMPLE_COUNT_1_BIT;
		case 2:
			return VK_SAMPLE_COUNT_2_BIT;
		case 4:
			return VK_SAMPLE_COUNT_4_BIT;
		case 8:
			return VK_SAMPLE_COUNT_8_BIT;
		case 16:
			return VK_SAMPLE_COUNT_16_BIT;
		case 32:
			return VK_SAMPLE_COUNT_32_BIT;
		case 64:
			return VK_SAMPLE_COUNT_64_BIT;
		default:
			RE_ASSERT(false, "Error Sample Count.");
		}
		return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
	}

	static VkDescriptorType ToVulkan(ResourceType type)
	{
		switch (type)
		{
		case ResourceType::UniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case ResourceType::DynamicUniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		case ResourceType::Texture:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		case ResourceType::Sampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;
		case ResourceType::CombinedImageSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		default:
			break;
		}
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}

	static VkImageLayout ToVulkan(AttachmentLayout layout)
	{
		switch (layout)
		{
		case AttachmentLayout::Undefined:
			return VK_IMAGE_LAYOUT_UNDEFINED;
		case AttachmentLayout::Present:
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		case AttachmentLayout::BlitSrc:
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case AttachmentLayout::BlitDst:
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case AttachmentLayout::Attachment:
			return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
		case AttachmentLayout::ColorBuffer:
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case AttachmentLayout::DepthBuffer:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case AttachmentLayout::General:
			return VK_IMAGE_LAYOUT_GENERAL;
		case AttachmentLayout::ReadOnlyColor:
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		case AttachmentLayout::ReadOnlyDepth:
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		default:
			RE_ASSERT(false, "Error Type Image Layout.");
		}
		return VK_IMAGE_LAYOUT_MAX_ENUM;
	}

	static VkFilter ToVulkan(Filter filter)
	{
		switch (filter)
		{
		case Filter::Linear:
			return VK_FILTER_LINEAR;
		case Filter::Nearest:
			return VK_FILTER_NEAREST;
		case Filter::Cube:
			return VK_FILTER_CUBIC_IMG;
		default:
			RE_ASSERT(false, "Error Type Filter.");
		}
		return VK_FILTER_MAX_ENUM;
	}

	static VkPolygonMode ToVulkan(PolygonMode mode)
	{
		switch (mode)
		{
		case PolygonMode::Triangle:
			return VK_POLYGON_MODE_FILL;
		case PolygonMode::Line:
			return VK_POLYGON_MODE_LINE;
		case PolygonMode::Point:
			return VK_POLYGON_MODE_POINT;
		default:
			RE_ASSERT(false, "Error Type Polygon Mode.");
		}
		return VK_POLYGON_MODE_MAX_ENUM;
	}

	static VkPrimitiveTopology ToVulkan(PrimitiveTopology topology)
	{
		switch (topology)
		{
		case PrimitiveTopology::TriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case PrimitiveTopology::TriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case PrimitiveTopology::LineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case PrimitiveTopology::LineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case PrimitiveTopology::PointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		default:
			RE_ASSERT(false, "Error Type Primitive Topology.");
		}
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

}