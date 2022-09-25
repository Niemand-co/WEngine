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
		default:
			RE_ASSERT(false, "Error Type Format.");
		}
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
	}

	static VkShaderStageFlagBits ToVulkan(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderStage::fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderStage::compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderStage::geometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		default:
			RE_ASSERT(false, "Error Type Shader Stage.");
		}
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
	}

}