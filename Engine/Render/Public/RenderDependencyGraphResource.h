#pragma once
#include "Render/Public/RenderDependencyGraph.h"

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

class WRDGTexture : public WRDGResource
{
public:

	virtual ~WRDGTexture() = default;

private:

	WRDGTexture(const WRDGTextureDesc& inDesc, const char* inName)
		: Desc(inDesc), Name(inName)
	{
	}

private:

	const WEngine::WString Name;

	const WRDGTextureDesc Desc;

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

	const WEngine::WString Name;

	const WRDGBufferDesc Desc;

	WEngine::WArray<WEngine::WPair<WRDGPassHandle, uint32>> States;

};