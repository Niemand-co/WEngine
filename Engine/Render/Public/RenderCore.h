#pragma once
#include "Utils/Container/Public/WArray.h"

enum class EShadingPath : uint8
{
	Forward,
	Deferred,
	ForwardPlus,
};

enum class EVertexInputType : uint8
{
	Default,
	PositionOnly,
	PositionAndNormal,
	PositionAndColor,
};

enum class Format
{
	R8_SInt = 0,
	R8_UInt,
	R8_UNorm,
	R8_SNorm,
	R8G8_SFloat,
	R8G8_SInt,
	R8G8_UInt,
	R8G8_UNorm,
	R8G8_SNorm,
	R8G8B8_SInt,
	R8G8B8_UInt,
	R8G8B8_UNorm,
	R8G8B8_SNorm,
	A8R8G8B8_SInt,
	A8R8G8B8_UInt,
	A8R8G8B8_UNorm,
	A8R8G8B8_SNorm,
	R16_SFloat,
	R16_SInt,
	R16_UInt,
	R16G16_SFloat,
	R16G16_SInt,
	R16G16_UInt,
	R16G16B16_SFloat,
	R16G16B16_SInt,
	R16G16B16_UInt,
	A16R16G16B16_SFloat,
	A16R16G16B16_SInt,
	A16R16G16B16_UInt,
	R32_SFloat,
	R32_SInt,
	R32_UInt,
	R32G32_SFloat,
	R32G32_SInt,
	R32G32_UInt,
	R32G32B32_SFloat,
	R32G32B32_SInt,
	R32G32B32_UInt,
	A32R32G32B32_SFloat,
	A32R32G32B32_SInt,
	A32R32G32B32_UInt,
	D32_SFloat,
	D16_Unorm,
	S8_UINT,
	B8G8R8A8_UNorm,
	D16_UNORM_S8_UINT,
	D24_UNORM_S8_UINT,
	D32_SFLOAT_S8_UINT
};

enum class ColorSpace
{
	SRGB_Linear = 0,

};

enum class PresentMode
{
	Immediate = 0,
	FIFO,
	FIFO_Relaxed,
	Mailbox,
};

struct Extent
{
	uint32 width = 0;
	uint32 height = 0;
	uint32 depth = 1;
};

struct IntRect
{
	struct IntCorner
	{
		int X, Y;
	};

	IntCorner Min;
	IntCorner Max;
};

enum class Dimension
{
	Texture1D = 0,
	Texture2D,
	Texture3D,
	TextureCUBE,
	Texture1DARRAY,
	Texture2DARRAY,
	TextureCUBEARRAY,
};

struct QueueProperty
{
	unsigned int QUEUE_SUPPORT = 0u;
	unsigned int count = 0u;
};

enum class MemoryType
{
	HostMemory = 0,
	LocalMemory,
};

struct MemoryTypeSupport
{
	MemoryType type;
	unsigned short properties;
	uint64_t size;
};

struct GPUFeature
{
	union
	{
		unsigned short sign;
		struct { char PHYSICAL_DEVICE_TYPE, QUEUE_SUPPORT; };
	};
	unsigned short SHDAER_SUPPORT;
	WEngine::WArray<MemoryTypeSupport*> memorySupports;
	WEngine::WArray<QueueProperty*> queueProperties;
	size_t minUBOAlignment;
};

enum class RHIQueueType : uint8
{
	Graphics = 0x01 << 0,
	Compute  = 0x01 << 1,
	Blit     = 0x01 << 2,
	Present  = 0x10 << 1,
};

struct QueueStack
{
	unsigned int count;
	unsigned int index;
	RHIQueueType type;
};

struct ImageSupport
{
	Format format;
	ColorSpace colorSpace;
};

enum class EncoderType
{
	Graphics = 0,
	Compute,
};

enum class ECompareOP : uint8
{
	Greater = 0,
	GE,
	Less,
	LE,
	Euqal,
	NotEqual,
	Always,
	Never
};

enum class EStencilFailedOP : uint8
{
	Keep = 0,
	Replace,
	Zero
};

enum class EDepthFailedOP : uint8
{
	Keep = 0,
	Replace,
	Zero
};

enum class EBlendFactor : uint8
{
	FactorOne,
	FactorZero,
	FactorSrcAlpha,
	FactorDstAlpha,
	FactorOneMinusSrcAlpha,
	FactorOneMinusDstAlpha,
	FactorConstantAlpha,
	FactorOneMinusConstantAlpha,
};

enum class EBlendOP : uint8
{
	BlendAdd = 0,
	BlendSubtract,
	BlendReverse_Subtract,
	BlendMin,
	BlendMax,
};

enum class AttachmentLoadOP
{
	Load = 0,
	DontCare,
	Clear,
};

enum class AttachmentStoreOP
{
	Store = 0,
	DontCare,
};

enum class AttachmentLayout
{
	Undefined = 0,
	Present,
	BlitSrc,
	BlitDst,
	Attachment,
	ColorBuffer,
	DepthBuffer,
	General,
	ReadOnlyDepth,
	ReadOnlyColor,
};

enum class ResourceType
{
	UniformBuffer = 0,
	DynamicUniformBuffer,
	Texture,
	Sampler,
	CombinedImageSampler,
};

enum class EFilter : uint8
{
	Linear = 0,
	Nearest,
	Cube,
};

enum class EPolygonMode : uint8
{
	Triangle = 0,
	Line,
	Point,
};

enum class EPrimitiveTopology : uint8
{
	TriangleList = 0,
	TriangleStrip,
	LineList,
	LineStrip,
	PointList,
};

enum class ECullMode : uint8
{
	None,
	Front,
	Back,
	FrontAndBack,
};

enum class EShaderStage : uint8
{
	Vertex = 0,
	Geometry = 1,
	Pixel = 2,
};

enum class EVertexElementType : uint32
{
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_PackedNormal,
	VET_Short2,
	VET_Short4,
	VET_Short2N,
	VET_Short4N,
	VET_Half2,
	VET_Half4,
	VET_UShort2,
	VET_UShort4,
	VET_UShort2N,
	VET_UShort4N,
	VET_URGB10A2N,
	VET_UInt
};

enum class EPassFeature : uint8
{
	Default,
	PositionOnly,
	PositionAndNormal,
};