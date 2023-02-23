#pragma once

class WRDGAllocator : public WEngine::Allocator<6>
{

};

template<typename HandleType>
class WRDGHandleBitArray : public WEngine::WBitArray
{
public:

	WRDGHandleBitArray() = default;

	WRDGHandleBitArray(uint32 Count, bool Value)
		: WEngine::WBitArray(Count, Value)
	{
	}

	WEngine::WBitReference operator[](HandleType Handle)
	{
		return WEngine::WBitArray::operator[](Handle.Index);
	}

	WEngine::WConstBitReference operator[](HandleType Handle) const
	{
		return WEngine::WBitArray::operator[](Handle.Index);
	}

};

template<typename InObjectType, typename IndexType>
class WRDGHandle
{
public:

	typedef InObjectType ObjectType;

	WRDGHandle() = default;

	explicit WRDGHandle(IndexType inIndex)
		: Index(inIndex)
	{
	}

	~WRDGHandle() = default;

	bool IsValid() { return Index != NullIndex; }
	bool operator==(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index == other.Index; }
	bool operator!=(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index != other.Index; }
	bool operator<=(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index <= other.Index; }
	bool operator>=(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index >= other.Index; }
	bool operator<(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index < other.Index; }
	bool operator>(const WRDGHandle<InObjectType, IndexType>& other) { RE_ASSERT(IsValid(), "Handle Invalid."); return Index > other.Index; }

	IndexType operator-(const WRDGHandle& other)
	{
		return Index - other.Index;
	}

	WRDGHandle& operator++()
	{
		Index++;
		return *this;
	}

	WRDGHandle& operator--()
	{
		Index--;
		return *this;
	}

public:

	static constexpr IndexType NullIndex = WEngine::NumericLimits<IndexType>::Max();

	IndexType Index = NullIndex;

};

template<typename HandleType>
class WRDGHandleRegistry
{
public:

	typedef HandleType::ObjectType ObjectType;

	WRDGHandleRegistry() = default;

	~WRDGHandleRegistry() = default;

	void Insert(ObjectType* Object)
	{
		Object->Handle = HandleType((uint16)Array.Size());
		Array.Push(Object);
	}

	template<typename AllocObjectType, typename... Args>
	AllocObjectType* Allocate(Args... args)
	{
		AllocObjectType* object = (AllocObjectType*)WRDGAllocator::Get()->Allocate(sizeof(AllocObjectType));
		::new (object) AllocObjectType(args...);
		Insert(static_cast<ObjectType*>(object));
		return object;
	}

	ObjectType* operator[](HandleType handle)
	{
		return Array[handle.Index];
	}

	const ObjectType* operator[](HandleType handle) const
	{
		return Array[handle.Index];
	}

	HandleType Begin() const
	{
		return HandleType(0);
	}

	HandleType End() const
	{
		return HandleType(Array.Size());
	}

private:

	WEngine::WArray<ObjectType*> Array;

};

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

	HostReading         = 1 << 0,
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

	ReadOnly            = HostReading | Present | VertexOrIndexBuffer | SRVCompute | SRVGraphics | CopySrc,
	WriteOnly           = RTV | CopyDst,

	Readable            = ReadOnly | UAV,
	Writable            = WriteOnly | UAV,
};
ENUM_CLASS_FLAGS(EAccess)

enum class EUniformBaseType : uint16
{
	UB_INVALID = 0,
	
	UB_BOOL,
	UB_INT32,
	UB_UINT32,
	UB_FLOAT32,
	
	UB_TEXTURE,
	UB_SRV,
	UB_UAV,
	UB_SAMPLER,

	UB_RDG_TEXTURE,
	UB_RDG_TEXTURE_ACCESS,
	UB_RDG_TEXTURE_SRV,
	UB_RDG_TEXTURE_UAV,
	UB_RDG_BUFFER,
	UB_RDG_BUFFER_SRV,
	UB_RDG_BUFFER_UAV,

	UB_RTV,
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

struct WRDGTerxtureSubresourceLayout
{
	WRDGTerxtureSubresourceLayout(uint32 InMipCount = 1u, uint32 InLayerCount = 1u, uint32 InPlaneCount = 1u)
		: MipCount(InMipCount), LayerCount(InLayerCount), PlaneCount(InPlaneCount)
	{
	}

	uint32 MipCount;

	uint32 LayerCount;

	uint32 PlaneCount;
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

	WRDGTextureDesc(Format inFormat, Extent inExtent, EDimension inDimension, ClearValue inClearValue, uint8 inMipCount, uint8 inSampleCount, uint32 inLayerCount)
		: format(inFormat), extent(inExtent), dimension(inDimension), clearValue(inClearValue), mipCount(inMipCount), sampleCount(inSampleCount), layerCount(inLayerCount)
	{
	}

	static WRDGTextureDesc GetTexture2DDesc(Format inFormat, Extent inExtent, ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2D, inClearValue, inMipCount, inSampleCount, 1);
	}

	static WRDGTextureDesc GetTexture2DArrayDesc(Format inFormat, Extent inExtent, ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, uint32 inLayerCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2DArray, inClearValue, inMipCount, inSampleCount, inLayerCount);
	}

	static WRDGTextureDesc GetTexture3DDesc(Format inFormat, Extent inExtent, ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture3D, inClearValue, inMipCount, inSampleCount, 1);
	}

	static WRDGTextureDesc GetTextureCubeDesc(Format inFormat, Extent inExtent, ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCube, inClearValue, inMipCount, inSampleCount, 1);
	}

	static WRDGTextureDesc GetTextureCubeArrayDesc(Format inFormat, Extent inExtent, ClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, uint32 inLayerCount = 1)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCubeArray, inClearValue, inMipCount, inSampleCount, inLayerCount);
	}

	bool IsTextureCube() const
	{
		return dimension == EDimension::TextureCube || dimension == EDimension::TextureCubeArray;
	}

	bool IsTextureArray() const
	{
		return dimension == EDimension::Texture2DArray || dimension == EDimension::TextureCubeArray;
	}

	bool IsTexture2D() const
	{
		return dimension == WRDGTextureDesc::EDimension::Texture2D || dimension == WRDGTextureDesc::EDimension::Texture2DArray;
	}

	bool IsStencilFormat() const
	{
		return format == Format::D16_UNORM_S8_UINT || format == Format::D24_UNORM_S8_UINT || format == Format::D32_SFLOAT_S8_UINT;
	}

	bool IsDepthFormat() const
	{
		return format == Format::D16_Unorm ||
			   format == Format::D16_UNORM_S8_UINT ||
			   format == Format::D24_UNORM_S8_UINT ||
			   format == Format::D32_SFloat ||
			   format == Format::D32_SFLOAT_S8_UINT;
	}

	WRDGTerxtureSubresourceLayout GetSubresourceLayout() const
	{
		return WRDGTerxtureSubresourceLayout(mipCount, layerCount * (IsTextureCube() ? 6 : 1), IsStencilFormat() ? 2 : 1);
	}

	Format format;
	Extent extent;
	EDimension dimension;
	ClearValue clearValue;
	uint8 mipCount;
	uint8 sampleCount;
	uint32 layerCount;
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

struct WRDGResourceState
{
	static bool IsTransitionRequired();

	static bool IsMergeAllowed(const WRDGResourceState& Previous, const WRDGResourceState& Next);

	EAccess Access = EAccess::Unknown;

	EPipeline Pipeline = EPipeline::Graphics;

	WRDGPassHandle FirstPass;

	WRDGPassHandle LastPass;
};