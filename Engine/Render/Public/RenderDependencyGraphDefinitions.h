#pragma once

class WRDGAllocator : public WEngine::Allocator<6>
{

};

template<typename ObjectType, typename IndexType>
class WRDGHandle;

template<typename HandleType>
class WRDGHandleRegistry;

class WRDGPass;
typedef WRDGHandle<WRDGPass, uint16> WRDGPassHandle;
typedef WEngine::WArray<WRDGPassHandle> WRDGPassHandleArray;
typedef WRDGHandleRegistry<WRDGPassHandle> WRDGPassRegistry;
typedef WRDGHandleBitArray<WRDGPassHandle> WRDGPassBitArray;

class WRDGTexture;
typedef WRDGHandle<WRDGTexture, uint16> WRDGTextureHandle;
typedef WEngine::WArray<WRDGTextureHandle> WRDGTextureHandleArray;
typedef WRDGHandleRegistry<WRDGTextureHandle> WRDGTextureRegistry;

class WRDGView;
typedef WRDGHandle<WRDGView, uint16> WRDGViewHandle;
typedef WEngine::WArray<WRDGViewHandle> WRDGViewHandleArray;
typedef WRDGHandleRegistry<WRDGViewHandle> WRDGViewRegistry;
typedef WRDGHandleBitArray<WRDGViewHandle> WRDGViewBitArray;

class WRDGBuffer;
typedef WRDGHandle<WRDGBuffer, uint16> WRDGBufferHandle;
typedef WEngine::WArray<WRDGBufferHandle> WRDGBufferHandleArray;
typedef WRDGHandleRegistry<WRDGBufferHandle> WRDGBufferRegistry;

enum class EAccess : uint16
{
	Unknown             = 0,

	HostVisible         = 1 << 0,
	Present             = 1 << 1,
	VertexOrIndexBuffer = 1 << 2,
	SRVCompute          = 1 << 3,
	SRVGraphics         = 1 << 4,
	CopySrc             = 1 << 5,

	UAVCompute          = 1 << 6,
	UAVGraphics         = 1 << 7,
	RTV                 = 1 << 8,
	CopyDst             = 1 << 9,

	SRV                 = SRVCompute | SRVGraphics,
	UAV                 = UAVCompute | UAVGraphics,

	ReadOnly            = HostVisible | Present | VertexOrIndexBuffer | SRVCompute | SRVGraphics | CopySrc,
	WriteOnly           = RTV | CopyDst,

	Readable            = ReadOnly | UAV,
	Writable            = WriteOnly | UAV,
};

enum class EUniformBaseType : uint16
{
	UB_INVALID = 0,
	
	UB_BOOL,
	UB_INT32,
	UB_UINT32,
	UB_FLOAT32,
	
	UB_SRV,
	UB_UAV,
	UB_SAMPLER,

	UB_RDG_TEXTURE_SRV,
	UB_RDG_TEXTURE_UAV,
	UB_RDG_BUFFER_SRV,
	UB_RDG_BUFFER_UAV,
};

enum class EPassFlag : uint16
{
	None = 0,

	Raster = 1 << 0,
	Compute = 1 << 1,
	AsyncCompute = 1 <<2,
	Copy = 1 << 3,

	NeverCull = 1 << 4,
	Skip = 1 << 5,

};

enum class EPipeline : uint8
{
	None = 0,
	Graphics = 1 << 0,
	AsyncCompute = 1 << 1,
};

struct WRDGTextureDesc
{
	enum class EDimension
	{
		Texture2D,
		Texture2DArray,
		Texture3D,
		TextureCube,
		TextureCubeArray
	};

	WRDGTextureDesc(Format inFormat, Extent inExtent, EDimension inDimension, WEngine::ClearValue inClearValue, uint8 inMipCount, uint8 inSampleCount)
		: format(inFormat), extent(inExtent), dimension(inDimension), clearValue(inClearValue), mipCount(inMipCount), sampleCount(inSampleCount)
	{
	}

	static WRDGTextureDesc GetTexture2DDesc(Format inFormat, Extent inExtent, WEngine::ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2D, inClearValue, inMipCount, inSampleCount);
	}

	static WRDGTextureDesc GetTexture2DArrayDesc(Format inFormat, Extent inExtent, WEngine::ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2DArray, inClearValue, inMipCount, inSampleCount);
	}

	static WRDGTextureDesc GetTexture3DDesc(Format inFormat, Extent inExtent, WEngine::ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture3D, inClearValue, inMipCount, inSampleCount);
	}

	static WRDGTextureDesc GetTextureCubeDesc(Format inFormat, Extent inExtent, WEngine::ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCube, inClearValue, inMipCount, inSampleCount);
	}

	static WRDGTextureDesc GetTextureCubeArrayDesc(Format inFormat, Extent inExtent, WEngine::ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCubeArray, inClearValue, inMipCount, inSampleCount);
	}

	Format format;
	Extent extent;
	EDimension dimension;
	WEngine::ClearValue clearValue;
	uint8 mipCount;
	uint8 sampleCount;
};

struct WRDGBufferDesc
{
	enum class EBufferType
	{
		VertexBuffer,
		IndexBuffer,
		StructuredBuffer
	};

	WRDGBufferDesc(uint32 inByteStride, uint32 inElementCount, EBufferType inBufferType, EBufferUsageFlags inBufferUsage)
		: ByteStride(inByteStride), ElementCount(inElementCount), BufferType(inBufferType), BufferUsage(inBufferUsage)
	{
	}

	static WRDGBufferDesc GetVertexBufferDesc(uint32 inByteStride, uint32 inElementCount, EBufferUsageFlags inBufferUsage)
	{
		return WRDGBufferDesc(inByteStride, inElementCount, EBufferType::VertexBuffer, inBufferUsage);
	}

	static WRDGBufferDesc GetIndexBufferDesc(uint32 inByteStride, uint32 inElementCount, EBufferUsageFlags inBufferUsage)
	{
		return WRDGBufferDesc(inByteStride, inElementCount, EBufferType::IndexBuffer, inBufferUsage);
	}

	static WRDGBufferDesc GetStructuredBufferDesc(uint32 inByteStride, uint32 inElementCount, EBufferUsageFlags inBufferUsage)
	{
		return WRDGBufferDesc(inByteStride, inElementCount, EBufferType::StructuredBuffer, inBufferUsage);
	}

	uint32 ByteStride;

	uint32 ElementCount;

	EBufferType BufferType;

	EBufferUsageFlags BufferUsage;
};