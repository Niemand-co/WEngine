#pragma once

struct RHITextureDesc
{
public:

	static RHITextureDesc CreateTexture2D(EFormat InFormat, FClearValue InClearValue, FExtent InExtent, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
	{
		return RHITextureDesc(InFormat, EDimension::Texture2D, InClearValue, InExtent, 1, InNumMips, InNumSamples, InFlags);
	}

	static RHITextureDesc CreateTexture2DArray(EFormat InFormat, FClearValue InClearValue, FExtent InExtent, uint16 InArraySize, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
	{
		return RHITextureDesc(InFormat, EDimension::Texture2DArray, InClearValue, InExtent, InArraySize, InNumMips, InNumSamples, InFlags);
	}

	static RHITextureDesc CreateTexture3D(EFormat InFormat, FClearValue InClearValue, FExtent InExtent, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
	{
		return RHITextureDesc(InFormat, EDimension::Texture3D, InClearValue, InExtent, 1, InNumMips, InNumSamples, InFlags);
	}

	static RHITextureDesc CreateTextureCube(EFormat InFormat, FClearValue InClearValue, FExtent InExtent, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
	{
		return RHITextureDesc(InFormat, EDimension::TextureCube, InClearValue, InExtent, 1, InNumMips, InNumSamples, InFlags);
	}

	static RHITextureDesc CreateTextureCubeArray(EFormat InFormat, FClearValue InClearValue, FExtent InExtent, uint16 InArraySize, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
	{
		return RHITextureDesc(InFormat, EDimension::TextureCubeArray, InClearValue, InExtent, InArraySize, InNumMips, InNumSamples, InFlags);
	}

	RHITextureDesc(EFormat InFormat, EDimension InDimension, FClearValue InClearValue, FExtent InExtent, uint16 InArraySize, uint8 InNumMips, uint8 InNumSamples, ETextureCreateFlags InFlags)
		: Format(InFormat),
		  Dimension(InDimension),
		  ClearValue(InClearValue),
		  Extent(InExtent),
		  ArraySize(InArraySize),
		  NumMips(InNumMips),
		  NumSamples(InNumSamples),
		  Flags(InFlags),
		  BulkData(nullptr),
		  BulkDataSize(0)
	{
	}

public:

	EFormat Format;

	EDimension Dimension;

	FClearValue ClearValue;

	FExtent Extent;

	uint16 ArraySize;

	uint8 NumMips;

	uint8 NumSamples;

	EAccess InitialState = EAccess::Unknown;

	ETextureCreateFlags Flags = ETextureCreateFlags::TextureCreate_None;

	void *BulkData;

	uint32 BulkDataSize;

};

class RHITexture : public RHIViewableResource
{
public:

	virtual ~RHITexture() = default;

	inline const RHITextureDesc& GetDesc() const { return Desc; }

	bool IsStencilFormat() const
	{
		return Desc.Format == EFormat::D16_UNORM_S8_UINT || Desc.Format == EFormat::D24_UNORM_S8_UINT || Desc.Format == EFormat::D32_SFLOAT_S8_UINT;
	}

	bool IsDepthFormat() const
	{
		return Desc.Format == EFormat::D16_Unorm ||
			   Desc.Format == EFormat::D16_UNORM_S8_UINT ||
			   Desc.Format == EFormat::D24_UNORM_S8_UINT ||
			   Desc.Format == EFormat::D32_SFloat ||
			   Desc.Format == EFormat::D32_SFLOAT_S8_UINT;
	}

	virtual void* GetTextureRHIBase() { return nullptr; }

protected:

	RHITexture(RHITextureDesc InDesc)
		: RHIViewableResource(ERHIResourceType::RRT_Texture, InDesc.InitialState),
		  Desc(InDesc)
	{
	}

protected:

	RHITextureDesc Desc;

};