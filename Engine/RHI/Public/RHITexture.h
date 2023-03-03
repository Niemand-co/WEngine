#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;
class RHIContext;

class RHITexture : public RHIResource
{
public:

	virtual ~RHITexture() = default;

	uint32 GetMipCount() const { return MipCount; }

	virtual uint32 GetWidth() const = 0;

	virtual uint32 GetHeight() const = 0;

	virtual uint32 GetDepth() const = 0;

	virtual uint32 GetLayerCount() const = 0;

	Format GetFormat() const { return PixelFormat; }

	ClearValue GetClearValue() const { return clearValue; }

	virtual Dimension GetDimension() const = 0;

	uint32 GetSampleCount() const { return SampleCount; }

	ETextureCreateFlags GetFlags() const { return Flags; }

	virtual void* GetTextureRHIBase() { return nullptr; }

protected:

	RHITexture(Format InPixelFormat, uint32 inMipCount, ClearValue InClearValue, uint32 InSampleCount, ETextureCreateFlags InFlags)
		: PixelFormat(InPixelFormat),
		  MipCount(inMipCount),
		  clearValue(InClearValue),
		  SampleCount(InSampleCount),
		  Flags(InFlags)
	{
	}

protected:

	uint32 MipCount;

	ClearValue clearValue;

	Format PixelFormat;

	uint32 SampleCount;

	ETextureCreateFlags Flags;

};

class RHITexture2D : public RHITexture
{
public:

	virtual ~RHITexture2D() = default;

	virtual uint32 GetWidth() const override { return Width; }

	virtual uint32 GetHeight() const override { return Height; }

	virtual uint32 GetDepth() const override { return 0; }

	virtual uint32 GetLayerCount() const override { return 1; }

	virtual Dimension GetDimension() const override { return Dimension::Texture2D; }

protected:

	RHITexture2D(Format InPixelFormat, uint32 inWidth, uint32 inHeight, uint32 inMipCount, ClearValue InClearValue, uint32 SampleCount, ETextureCreateFlags Flags)
		: Width(inWidth), Height(inHeight), RHITexture(InPixelFormat, inMipCount, InClearValue, SampleCount, Flags)
	{
	}

protected:

	uint32 Width;

	uint32 Height;

};

class RHITexture2DArray : public RHITexture2D
{
public:

	virtual ~RHITexture2DArray() = default;

	virtual uint32 GetLayerCount() const override { return LayerCount; }

	virtual Dimension GetDimension() const override { return Dimension::Texture2DARRAY; }

protected:

	RHITexture2DArray(Format InPixelFormat, uint32 inWidth, uint32 inHeight, uint32 inMipCount, uint32 inLayerCount, ClearValue InClearValue, uint32 SampleCount, ETextureCreateFlags Flags)
		: RHITexture2D(InPixelFormat, inWidth, inHeight, inMipCount, InClearValue, SampleCount, Flags), LayerCount(inLayerCount)
	{
	}

protected:

	uint32 LayerCount;

};

class RHITexture3D : public RHITexture
{
public:

	virtual ~RHITexture3D() = default;

	virtual uint32 GetWidth() const override { return Width; }

	virtual uint32 GetHeight() const override { return Height; }

	virtual uint32 GetDepth() const override { return Depth; }

	virtual uint32 GetLayerCount() const override { return 1; }

	virtual Dimension GetDimension() const override { return Dimension::Texture3D; }

protected:

	RHITexture3D(Format InPixelFormat, uint32 inWidth, uint32 inHeight, uint32 inDepth, uint32 inMipCount, ClearValue InClearValue, uint32 SampleCount, ETextureCreateFlags Flags)
		: Width(inWidth), Height(inHeight), Depth(inDepth), RHITexture(InPixelFormat, inMipCount, InClearValue, SampleCount, Flags)
	{
	}

protected:

	uint32 Width;

	uint32 Height;

	uint32 Depth;

};