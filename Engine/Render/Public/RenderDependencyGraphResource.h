#pragma once
#include "Render/Public/RenderDependencyGraph.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

struct WRDGResourceState
{
	static bool IsTransitionRequired();

	static bool IsMergeAllowed(const WRDGResourceState& Previous, const WRDGResourceState& Next);

	EAccess Access = EAccess::Unknown;

	EPipeline Pipeline = EPipeline::Graphics;

	WRDGPassHandle FirstPass;

	WRDGPassHandle LastPass;
};

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

	WRDGPassHandle LastProducer;

	WEngine::WSharedPtr<class WRHIResource> RHI;

};

class WRDGTextureSubresource
{
public:

	WRDGTextureSubresource(uint32 inMipLevelCount = 1u, uint32 inBaseMipLevel = 0u, uint32 inArrayCount = 1u, uint32 inBaseArray = 0u)
		: MipLevelCount(inMipLevelCount), BaseMipLevel(inBaseMipLevel), ArrayCount(inArrayCount), BaseArray(inBaseArray)
	{
	}

private:

	uint32 MipLevelCount;

	uint32 BaseMipLevel;

	uint32 ArrayCount;

	uint32 BaseArray;

};

class WRDGTexture : public WRDGResource
{
public:

	virtual ~WRDGTexture() = default;

	WEngine::WArray<WRDGResourceState*>& GetMergeState() { return MergeState; }

private:

	WRDGTexture(const WRDGTextureDesc& inDesc, const char* inName)
		: Desc(inDesc), Name(inName)
	{
	}

private:

	WRDGTextureHandle Handle;

	const WEngine::WString Name;

	const WRDGTextureDesc Desc;

	WEngine::WArray<WRDGResourceState*> MergeState;

	friend class WRDGBuilder;
	friend class WRDGTextureRegistry;

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

	WEngine::WArray<WEngine::WPair<WRDGPassHandle, uint32>> States;

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