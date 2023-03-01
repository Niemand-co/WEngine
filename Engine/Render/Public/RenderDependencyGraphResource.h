#pragma once
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

	WRDGPassHandle FirstPass;

	WRDGPassHandle LastPass;

	friend class WRDGBuilder;

};

class WRDGPooledTexture
{
public:

	WRDGPooledTexture();

	~WRDGPooledTexture();

private:

	class WRHITexture *TextureRHI;

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
		  Layout(inDesc.GetSubresourceLayout()),
		  RHI(nullptr)
	{
	}

private:

	WRDGTextureHandle Handle;

	const WEngine::WString Name;

	const WRDGTextureDesc Desc;

	class RHITexture* RHI;

	const WRDGTerxtureSubresourceLayout Layout;

	WEngine::WArray<WRDGResourceState> States;

	WEngine::WArray<WRDGResourceState*> MergeState;

	friend class WRDGBuilder;
	friend WRDGTextureRegistry;
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
		: Desc(inDesc),
		  Name(inName),
		  RHI(nullptr)
	{
	}

private:

	WRDGBufferHandle Handle;

	const WEngine::WString Name;

	class RHIBuffer *RHI;

	const WRDGBufferDesc Desc;

	WRDGResourceState State;

	WRDGResourceState *MergeState = nullptr;

	friend class WRDGBuilder;
	friend WRDGBufferRegistry;

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

	WRDGTexture *Texture;

};

class WRDGTextureUAVDesc : public RHITextureViewDescriptor
{
public:

	WRDGTextureUAVDesc() = default;

	virtual ~WRDGTextureUAVDesc() = default;

public:

	WRDGTexture *Texture;

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

private:

	WTextureViewRHIRef RHI;

	friend class WRDGBuilder;
	friend WRDGViewRegistry;

};

class WRDGTextureUAV : public WRDGUnorderedAccessView
{
public:

	const WRDGTextureUAVDesc Desc;

private:

	WRDGTextureUAV(const WRDGTextureUAVDesc& inDesc)
		: Desc(inDesc)
	{
	}

private:

	WTextureViewRHIRef RHI;

	friend class WRDGBuilder;
	friend WRDGViewRegistry;

};