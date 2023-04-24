#pragma once
#include "Render/Public/RenderResource.h"

class RHITexture;

class WTexture : public RenderResource
{
public:

	WTexture() = default;

	virtual ~WTexture() = default;

private:

	WEngine::WString Name;

};

class WTexture2D : public WTexture
{
public:

	WTexture2D(uint32 inWidth, uint32 inHeight, EFormat inFormat)
		: Width(inWidth), Height(inHeight), format(inFormat)
	{
	}

	virtual ~WTexture2D() = default;

protected:

	uint32 Width;

	uint32 Height;

	EFormat format;

	WTextureRHIRef RHI;

};

class WTextureWithSRV : public WTexture2D
{
public:

	WTextureWithSRV(uint32 inWidth, uint32 inHeight, EFormat inFormat);

	virtual ~WTextureWithSRV() = default;

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};

class WTextureWithRTV : public WTexture2D
{
public:

	WTextureWithRTV();

	virtual ~WTextureWithRTV();

};