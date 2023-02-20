#pragma once
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

template<typename ElementType>
inline void InitResource(WEngine::WArray<ElementType>& ResourceArray, uint32 Num, const ElementType& Element = {})
{
	ResourceArray.Resize(Num);
	for (uint32 Index = 0; Index < Num; ++Index)
	{
		ResourceArray[Index] = Element;
	}
}

class WRDGResource
{
public:

	virtual ~WRDGResource() = default;

	bool IsExternal() const { return bExternal; }

	void* operator new(size_t size)
	{
		return WRDGAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WRDGAllocator::Get()->Deallocate(pData);
	}

protected:

	uint8 bExternal : 1;

	uint16 ReferenceCount = 0;

	WRDGPassHandle LastProducer;

	WEngine::WSharedPtr<class WRHIResource> RHI;

	friend class WRDGBuilder;

};

struct WRDGTextureSubresource
{
	WRDGTextureSubresource(uint32 InMipIndex = 0u, uint32 InLayerIndex = 0u, uint32 InPlaneIndex = 0u)
		: MipIndex(InMipIndex), LayerIndex(InLayerIndex), PlaneIndex(InPlaneIndex)
	{
	}

	uint32 MipIndex;

	uint32 LayerIndex;

	uint32 PlaneIndex;
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

struct WRDGTextureSubresourceRange
{
	WRDGTextureSubresourceRange()
		: MipIndex(0),
		  MipCount(1),
		  LayerIndex(0),
		  LayerCount(1),
		  PlaneIndex(0),
		  PlaneCount(1)
	{
	}

	WRDGTextureSubresourceRange(const WRDGTerxtureSubresourceLayout& Layout)
		: MipIndex(0),
		MipCount(Layout.MipCount),
		LayerIndex(0),
		LayerCount(Layout.LayerCount),
		PlaneIndex(0),
		PlaneCount(Layout.PlaneCount)
	{
	}

	uint32 MipIndex;

	uint32 LayerIndex;

	uint32 PlaneIndex;

	uint32 MipCount;

	uint32 LayerCount;

	uint32 PlaneCount;
};

class WRDGTexture : public WRDGResource
{
public:

	virtual ~WRDGTexture() = default;

	WEngine::WArray<WRDGResourceState*>& GetMergeState() { return MergeState; }

	template<typename LAMBDA>
	void EnumerateSubresource(WRDGTextureSubresourceRange Range, LAMBDA lambda)
	{
		for (uint32 MipIndex = 0; MipIndex < Range.MipCount; ++MipIndex)
		{
			for (uint32 LayerIndex = 0; LayerIndex < Range.LayerCount; ++LayerIndex)
			{
				for(uint32 PlaneIndex = 0; PlaneIndex < Range.PlaneCount; ++PlaneIndex)
				{
					lambda(MipIndex + Range.MipIndex + (LayerIndex + Range.LayerIndex) * Range.MipCount + (PlaneIndex + Range.PlaneIndex) * Range.MipCount * Range.LayerCount);
				}
			}
		}
	}

	WRDGTextureSubresourceRange GetSubresourceRange() { return WRDGTextureSubresourceRange(Layout); }

private:

	WRDGTexture(const WRDGTextureDesc& inDesc, const char* inName)
		: Desc(inDesc),
		  Name(inName),
		  Layout(inDesc.GetSubresourceLayout())
	{
	}

private:

	WRDGTextureHandle Handle;

	const WEngine::WString Name;

	const WRDGTextureDesc Desc;

	const WRDGTerxtureSubresourceLayout Layout;

	WEngine::WArray<WRDGResourceState*> MergeState;

	friend class WRDGBuilder;
	friend class WRDGTextureRegistry;
	friend class WRDGParameterStruct;

};

class WRDGView : public WRDGResource
{
protected:

	WRDGView();

private:

	WRDGViewHandle Handle;

};

class WRDGBuffer : public WRDGResource
{
public:

	virtual ~WRDGBuffer() = default;

private:

	WRDGBuffer(const WRDGBufferDesc& inDesc, const char* inName)
		: Desc(inDesc), Name(inName)
	{
	}

private:

	WRDGBufferHandle Handle;

	const WEngine::WString Name;

	const WRDGBufferDesc Desc;

	WRDGResourceState *MergeState = nullptr;

	friend class WRDGBuilder;
	friend class WRDGBufferRegistry;

};

class WRDGShaderResourceView : public WRDGView
{

};

class WRDGUnorderedAccessView : public WRDGView
{

};

class WRDGTextureSRVDesc : public RHITextureViewDescriptor
{
public:

	WRDGTextureSRVDesc() = default;

	virtual ~WRDGTextureSRVDesc() = default;

	static WRDGTextureSRVDesc Get(WRDGTexture *inTexture)
	{
		WRDGTextureSRVDesc Desc;
		Desc.Texture = inTexture;
	}

	static WRDGTextureSRVDesc GetForMipLevel(WRDGTexture *inTexture, uint32 inMipLevel)
	{
		WRDGTextureSRVDesc Desc;
		Desc.Texture = inTexture;
		Desc.baseMipLevel = inMipLevel;
	}

	static WRDGTextureSRVDesc GetWithPixelFormat(WRDGTexture* inTexture, Format PixelFormat)
	{
		WRDGTextureSRVDesc Desc;
		Desc.Texture = inTexture;
		Desc.format = PixelFormat;
	}

public:

	WRDGTexture* Texture;

};

class WRDGTextureSRV : public WRDGShaderResourceView
{
public:

	const WRDGTextureSRVDesc Desc;

private:

	WRDGTextureSRV(const WRDGTextureSRVDesc& inDesc)
		: Desc(inDesc)
	{
	}

	friend class WRDGBuilder;
	friend class WRDGViewRegistry;

};

class WRDGTextureUAV : public WRDGUnorderedAccessView
{

};