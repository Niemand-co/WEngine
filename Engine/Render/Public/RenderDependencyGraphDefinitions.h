#pragma once
#include "Utils/Container/Public/WBitArray.h"
#include "Utils/Allocator/Public/Allocator.h"

class WRDGHead
{
public:

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

};

class WRDGAllocator : public WRDGHead
{
public:

	template<typename ObjectType, typename ...Args>
	ObjectType* AllocateObject(Args... args)
	{
		TrackedAlloc<ObjectType>* Object = (TrackedAlloc<ObjectType>*)Allocate(sizeof(TrackedAlloc<ObjectType>));
		::new (Object) TrackedAlloc<ObjectType>(args...);
		TrackedObjects.Push(Object);
		return Object->Get();
	}

	void Clear()
	{
		for (TrackedBase* Alloc : TrackedObjects)
		{
			Alloc->~TrackedBase();
			Deallocate(Alloc);
		}
		TrackedObjects.Clear();
	}

	void* Allocate(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void Deallocate(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

	static WRDGAllocator* Get()
	{
		static WRDGAllocator* Allocator = new WRDGAllocator;
		return Allocator;
	}

private:

	struct TrackedBase
	{
		virtual ~TrackedBase() = default;
	};

	template<typename ObjectType>
	struct TrackedAlloc : TrackedBase
	{
		template<typename... Args>
		TrackedAlloc(Args... args) : Object(args...) {}

		ObjectType* Get() { return &Object; }

		ObjectType Object;
	};

	WEngine::WArray<TrackedBase*> TrackedObjects;

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
class WRDGHandle : public WRDGHead
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
class WRDGHandleRegistry : public WRDGHead
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
		AllocObjectType* object = WRDGAllocator::Get()->AllocateObject<AllocObjectType>(args...);
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

	void Clear()
	{
		Array.Clear();
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

	UB_STRUCT,
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

	WRDGTextureDesc() = default;

	WRDGTextureDesc(EFormat inFormat, FExtent inExtent, EDimension inDimension, FClearValue inClearValue, uint8 inMipCount, uint8 inSampleCount, uint32 inLayerCount, ETextureCreateFlags InFlags)
		: Format(inFormat), Extent(inExtent), dimension(inDimension), ClearValue(inClearValue), mipCount(inMipCount), sampleCount(inSampleCount), layerCount(inLayerCount), Flags(InFlags)
	{
	}

	static WRDGTextureDesc GetTexture2DDesc(EFormat inFormat, FExtent inExtent, FClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, ETextureCreateFlags InFlags = ETextureCreateFlags::TextureCreate_None)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2D, inClearValue, inMipCount, inSampleCount, 1, InFlags);
	}

	static WRDGTextureDesc GetTexture2DArrayDesc(EFormat inFormat, FExtent inExtent, FClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, uint32 inLayerCount = 1, ETextureCreateFlags InFlags = ETextureCreateFlags::TextureCreate_None)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture2DArray, inClearValue, inMipCount, inSampleCount, inLayerCount, InFlags);
	}

	static WRDGTextureDesc GetTexture3DDesc(EFormat inFormat, FExtent inExtent, FClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, ETextureCreateFlags InFlags = ETextureCreateFlags::TextureCreate_None)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::Texture3D, inClearValue, inMipCount, inSampleCount, 1, InFlags);
	}

	static WRDGTextureDesc GetTextureCubeDesc(EFormat inFormat, FExtent inExtent, FClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, ETextureCreateFlags InFlags = ETextureCreateFlags::TextureCreate_None)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCube, inClearValue, inMipCount, inSampleCount, 1, InFlags);
	}

	static WRDGTextureDesc GetTextureCubeArrayDesc(EFormat inFormat, FExtent inExtent, FClearValue inClearValue, uint8 inMipCount = 1, uint8 inSampleCount = 1, uint32 inLayerCount = 1, ETextureCreateFlags InFlags = ETextureCreateFlags::TextureCreate_None)
	{
		return WRDGTextureDesc(inFormat, inExtent, EDimension::TextureCubeArray, inClearValue, inMipCount, inSampleCount, inLayerCount, InFlags);
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
		return Format == EFormat::D16_UNORM_S8_UINT || Format == EFormat::D24_UNORM_S8_UINT || Format == EFormat::D32_SFLOAT_S8_UINT;
	}

	bool IsDepthFormat() const
	{
		return Format == EFormat::D16_Unorm ||
			   Format == EFormat::D16_UNORM_S8_UINT ||
			   Format == EFormat::D24_UNORM_S8_UINT ||
			   Format == EFormat::D32_SFloat ||
			   Format == EFormat::D32_SFLOAT_S8_UINT;
	}

	WRDGTerxtureSubresourceLayout GetSubresourceLayout() const
	{
		return WRDGTerxtureSubresourceLayout(mipCount, layerCount * (IsTextureCube() ? 6 : 1), IsStencilFormat() ? 2 : 1);
	}

	EFormat Format = EFormat::A16R16G16B16_SFloat;
	FExtent Extent;
	EDimension dimension = EDimension::Texture2D;
	FClearValue ClearValue;
	uint8 mipCount = 1;
	uint8 sampleCount = 1;
	uint32 layerCount = 1;
	ETextureCreateFlags Flags = ETextureCreateFlags::TextureCreate_None;
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

	bool operator==(const WRDGResourceState& other) const
	{
		return Access == other.Access && Pipeline == other.Pipeline;
	}
};