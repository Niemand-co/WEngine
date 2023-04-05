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
		case Format::S8_UINT:
			return VK_FORMAT_S8_UINT;
		case Format::B8G8R8A8_UNorm:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case Format::D16_UNORM_S8_UINT:
			return VK_FORMAT_D16_UNORM_S8_UINT;
		case Format::D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;
		case Format::D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;
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

	static VkCompareOp ToVulkan(ECompareOP op)
	{
		switch (op)
		{
		case ECompareOP::Greater:
			return VK_COMPARE_OP_GREATER;
		case ECompareOP::GE:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case ECompareOP::Less:
			return VK_COMPARE_OP_LESS;
		case ECompareOP::LE:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		case ECompareOP::Euqal:
			return VK_COMPARE_OP_EQUAL;
		case ECompareOP::NotEqual:
			return VK_COMPARE_OP_NOT_EQUAL;
		case ECompareOP::Always:
			return VK_COMPARE_OP_ALWAYS;
		case ECompareOP::Never:
			return VK_COMPARE_OP_NEVER;
		default:
			break;
		}
		return VK_COMPARE_OP_MAX_ENUM;
	}

	static VkStencilOp ToVulkan(EStencilFailedOP op)
	{
		switch (op)
		{
		case EStencilFailedOP::Keep:
			return VK_STENCIL_OP_KEEP;
		case EStencilFailedOP::Replace:
			return VK_STENCIL_OP_REPLACE;
		case EStencilFailedOP::Zero:
			return VK_STENCIL_OP_ZERO;
		default:
			RE_ASSERT(false, "Error Type Stencil Failed Operation");
		}
		return VK_STENCIL_OP_MAX_ENUM;
	}

	static VkBlendFactor ToVulkan(EBlendFactor factor)
	{
		switch (factor)
		{
		case EBlendFactor::FactorOne:
			return VK_BLEND_FACTOR_ONE;
		case EBlendFactor::FactorZero:
			return VK_BLEND_FACTOR_ZERO;
		case EBlendFactor::FactorSrcAlpha:
			return VK_BLEND_FACTOR_SRC_ALPHA;
		case EBlendFactor::FactorDstAlpha:
			return VK_BLEND_FACTOR_DST_ALPHA;
		case EBlendFactor::FactorOneMinusSrcAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case EBlendFactor::FactorOneMinusDstAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case EBlendFactor::FactorConstantAlpha:
			return VK_BLEND_FACTOR_CONSTANT_ALPHA;
		case EBlendFactor::FactorOneMinusConstantAlpha:
			return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		default:
			RE_ASSERT(false, "Error Type Blend Factor.");
		}
		return VK_BLEND_FACTOR_MAX_ENUM;
	}

	static VkBlendOp ToVulkan(EBlendOP op)
	{
		switch (op)
		{
		case EBlendOP::BlendAdd:
			return VK_BLEND_OP_ADD;
		case EBlendOP::BlendSubtract:
			return VK_BLEND_OP_SUBTRACT;
		case EBlendOP::BlendReverse_Subtract:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
		case EBlendOP::BlendMin:
			return VK_BLEND_OP_MIN;
		case EBlendOP::BlendMax:
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

	static VkFilter ToVulkan(EFilter filter)
	{
		switch (filter)
		{
		case EFilter::Linear:
			return VK_FILTER_LINEAR;
		case EFilter::Nearest:
			return VK_FILTER_NEAREST;
		case EFilter::Cube:
			return VK_FILTER_CUBIC_IMG;
		default:
			RE_ASSERT(false, "Error Type Filter.");
		}
		return VK_FILTER_MAX_ENUM;
	}

	static VkPolygonMode ToVulkan(EPolygonMode mode)
	{
		switch (mode)
		{
		case EPolygonMode::Triangle:
			return VK_POLYGON_MODE_FILL;
		case EPolygonMode::Line:
			return VK_POLYGON_MODE_LINE;
		case EPolygonMode::Point:
			return VK_POLYGON_MODE_POINT;
		default:
			RE_ASSERT(false, "Error Type Polygon Mode.");
		}
		return VK_POLYGON_MODE_MAX_ENUM;
	}

	static VkPrimitiveTopology ToVulkan(EPrimitiveTopology topology)
	{
		switch (topology)
		{
		case EPrimitiveTopology::TriangleList:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case EPrimitiveTopology::TriangleStrip:
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case EPrimitiveTopology::LineList:
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case EPrimitiveTopology::LineStrip:
			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case EPrimitiveTopology::PointList:
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		default:
			RE_ASSERT(false, "Error Type Primitive Topology.");
		}
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

	static VkCullModeFlags ToVulkan(ECullMode CullMode)
	{
		switch (CullMode)
		{
		case ECullMode::None:
			return VK_CULL_MODE_NONE;
		case ECullMode::Front:
			return VK_CULL_MODE_FRONT_BIT;
		case ECullMode::Back:
			return VK_CULL_MODE_BACK_BIT;
		case ECullMode::FrontAndBack:
			return VK_CULL_MODE_FRONT_AND_BACK;
		default:
			RE_ASSERT(false, "Error Type Cull Mode.");
		}
		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	}

	static VkFormat ToVulkan(EVertexElementType VertexType)
	{
		switch (VertexType)
		{
		case EVertexElementType::VET_Float1:
			return VK_FORMAT_R32_SFLOAT;
		case EVertexElementType::VET_Float2:
			return VK_FORMAT_R32G32_SFLOAT;
		case EVertexElementType::VET_Float3:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case EVertexElementType::VET_UByte4:
			return VK_FORMAT_R8G8B8A8_UINT;
		case EVertexElementType::VET_UByte4N:
		case EVertexElementType::VET_Color:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case EVertexElementType::VET_PackedNormal:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case EVertexElementType::VET_Short2:
			return VK_FORMAT_R16G16_SINT;
		case EVertexElementType::VET_Short4:
			return VK_FORMAT_R16G16B16A16_SINT;
		case EVertexElementType::VET_Short2N:
			return VK_FORMAT_R16G16_SNORM;
		case EVertexElementType::VET_Short4N:
			return VK_FORMAT_R16G16B16A16_SNORM;
		case EVertexElementType::VET_Half2:
			return VK_FORMAT_R16G16_SFLOAT;
		case EVertexElementType::VET_Half4:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case EVertexElementType::VET_UShort2:
			return VK_FORMAT_R16G16_UINT;
		case EVertexElementType::VET_UShort4:
			return VK_FORMAT_R16G16B16A16_UINT;
		case EVertexElementType::VET_UShort2N:
			return VK_FORMAT_R16G16_UNORM;
		case EVertexElementType::VET_UShort4N:
			return VK_FORMAT_R16G16B16A16_UNORM;
		case EVertexElementType::VET_URGB10A2N:
			return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
		case EVertexElementType::VET_UInt:
			return VK_FORMAT_R32_UINT;
		default:
			RE_ASSERT(false, "Error Type Vertex Type.");
		}
		return VK_FORMAT_MAX_ENUM;
	}

	static VkShaderStageFlags ToVulkan(EShaderStage Stage)
	{
		switch (Stage)
		{
		case EShaderStage::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case EShaderStage::Geometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		case EShaderStage::Pixel:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		default:
			RE_ASSERT(false, "Error Type Shader Stage.")
		}
		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}

}

enum class EState : int32
{
	None = 0,

	OutofData = -1,
	SurfaceLost = -2,
	Healthy = None,
};

struct ClearValue
{
	union
	{
		float Color[4];
		struct { float Depth; uint32 Stencil; };
	};
};

struct WResolveRect
{
	int32 X1;
	int32 Y1;
	int32 X2;
	int32 Y2;

	bool operator==(const WResolveRect& other) const
	{
		return X1 == other.X1 && Y1 == other.Y1 && X2 == other.X2 && Y2 == other.Y2;
	}
};

enum
{
	MaxVertexInputElementCount = 17,
	MaxVertexInputElementCount_NumBits = 5,
};

enum
{
	MaxSimultaneousRenderTargets = 8,
	MaxSimultaneousRenderTargets_NumBits = 4,
};

enum
{
	MaxGraphicsPipelineShaderNum = 3,
};

enum class ETextureCreateFlags : uint32
{
	TextureCreate_None                    = 0,
	TextureCreate_Presentable             = 1 << 0,
	TextureCreate_RenderTarget            = 1 << 1,
	TextureCreate_DepthStencil            = 1 << 2,
	TextureCreate_SRV                     = 1 << 3,
	TextureCreate_UAV                     = 1 << 4,
	TextureCreate_NoTiling                = 1 << 5,
	TextureCreate_CPUWritable             = 1 << 6,
	TextureCreate_CPUReadable             = 1 << 7,
	TextureCreate_Transient               = 1 << 8,
	TextureCreate_InputAttachmentReadable = 1 << 9,
	TextureCreate_Memoryless              = 1 << 10,
};
ENUM_CLASS_FLAGS(ETextureCreateFlags)

struct RHIBlendStateInitializer
{
	bool bEnableBlend = true;
	EBlendOP ColorBlendOp = EBlendOP::BlendAdd;
	EBlendFactor ColorSrcFactor = EBlendFactor::FactorOne;
	EBlendFactor ColorDstFactor = EBlendFactor::FactorZero;
	EBlendOP AlphaBlendOp = EBlendOP::BlendAdd;
	EBlendFactor AlphaSrcFactor = EBlendFactor::FactorOne;
	EBlendFactor AlphaDstFactor = EBlendFactor::FactorZero;
};

struct RHIDepthStencilStateInitializer
{
	bool bEnableDepthTest = true;
	bool bEnableDepthWrite = true;
	ECompareOP DepthCompareOp = ECompareOP::NotEqual;
	bool bEnableFrontStencil = false;
	ECompareOP FrontStencilCompareOp = ECompareOP::Always;
	EStencilFailedOP FrontStencilFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP FrontDepthFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP FrontStencilPassOp = EStencilFailedOP::Replace;
	bool bEnableBackStencil = false;
	ECompareOP BackStencilCompareOp = ECompareOP::Always;
	EStencilFailedOP BackStencilFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP BackDepthFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP BackStencilPassOp = EStencilFailedOP::Replace;
	uint8 StencilReadMask = 0xFF;
	uint8 StencilWriteMask = 0xFF;
};

struct RHIRasterizationStateInitializer
{
	ECullMode CullMode = ECullMode::None;
	EPolygonMode PolygonMode = EPolygonMode::Triangle;
	EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::TriangleList;
	bool bEnableDepthClamp = false;
	bool bEnableDiscard = false;
	bool bClockWise = false;
};

struct RHIMultiSampleStateInitializer
{
	bool bEnableSampleShading = false;
	uint32 SampleCount = 1;
	uint32 SampleMask = 0x00;
	float MinSampleShading = 0.f;
	bool bEnableAlphaToCoverage = false;
	bool bEnableAlphaToOne = false;
};

typedef class RHIVertexBuffer* WVertexBufferRHIRef;
typedef class RHIDynamicVertexBuffer* WDynamicVertexBufferRHIRef;
typedef class RHIIndexBuffer* WIndexBufferRHIRef;
typedef class RHIUniformBuffer* WUniformBufferRHIRef;
typedef class RHIDynamicUniformBuffer* WDynamicUniformBufferRHIRef;
typedef class RHITextureBuffer* WTextureBufferRHIRef;

typedef class RHIVertexShader* WVertexShaderRHIRef;
typedef class RHIPixelShader* WPixelShaderRHIRef;
typedef class RHIGeometryShader* WGeometryShaderRHIRef;
typedef class RHIComputeShader* WComputeShaderRHIRef;

typedef class RHITexture2D* WTexture2DRHIRef;
typedef class RHITexture2DArray* WTexture2DArrayRHIRef;
typedef class RHITexture3D* WTexture3DRHIRef;

typedef class RHITextureView* WTextureViewRHIRef;

typedef class RHIViewport* WViewportRHIRef;

typedef class RHIRenderPass* WRenderPassRHIRef;
typedef class RHIFramebuffer* WFramebufferRHIRef;
typedef class RHIPipelineStateObject* WPsoRHIRef;

typedef class RHIBlendState* WBlendStateRHIRef;
typedef class RHIDepthStencilState* WDepthStencilStateRHIRef;
typedef class RHIRasterizationState* WRasterizationStateRHIRef;
typedef class RHIMultiSampleState* WMultiSampleStateRHIRef;
typedef class RHIVertexInputState* WVertexInputStateRHIRef;