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

	virtual void* GetTextureRHIBase() { return nullptr; }

protected:

	RHITexture(uint32 inMipCount)
		: MipCount(inMipCount)
	{
	}

protected:

	uint32 MipCount;

};

class RHITexture2D : public RHITexture
{
public:

	virtual ~RHITexture2D() = default;

	virtual uint32 GetWidth() const override { return Width; }

	virtual uint32 GetHeight() const override { return Height; }

	virtual uint32 GetDepth() const override { return 0; }

	virtual uint32 GetLayerCount() const override { return 1; }

protected:

	RHITexture2D(uint32 inWidth, uint32 inHeight, uint32 inMipCount)
		: Width(inWidth), Height(inHeight), RHITexture(inMipCount)
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

protected:

	RHITexture2DArray(uint32 inWidth, uint32 inHeight, uint32 inMipCount, uint32 inLayerCount)
		: RHITexture2D(inWidth, inHeight, inMipCount), LayerCount(inLayerCount)
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

protected:

	RHITexture3D(uint32 inWidth, uint32 inHeight, uint32 inDepth, uint32 inMipCount)
		: Width(inWidth), Height(inHeight), Depth(inDepth), RHITexture(inMipCount)
	{
	}

protected:

	uint32 Width;

	uint32 Height;

	uint32 Depth;

};