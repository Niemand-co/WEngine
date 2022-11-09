#pragma once

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
	B8G8R8A8_UNorm,
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
	unsigned int width;
	unsigned int height;
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
	std::vector<MemoryTypeSupport*> memorySupports;
	std::vector<QueueProperty*> queueProperties;
};

enum class RHIQueueType
{
	Graphics = 0,
	Present,
	Compute,
	Blit,
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

enum class DepthCompareOP
{
	Greater = 0,
	GE,
	Less,
	LE,
};

enum class BlendFactor
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

enum class BlendOP
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

enum class Filter
{
	Linear = 0,
	Nearest,
	Cube,
};