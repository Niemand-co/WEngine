#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;
class RHIContext;

class RHITexture : public RHIResource
{
public:

	virtual ~RHITexture() = default;

	//virtual void LoadData(const WEngine::WString& path, RHIContext *context) = 0;

};

class RHITexture2D : public RHITexture
{
public:

	virtual ~RHITexture2D() = default;

	uint32 GetWidth() const { return Width; }

	uint32 GetHeight() const { return Height; }

protected:

	RHITexture2D(uint32 inWidth, uint32 inHeight)
		: Width(inWidth), Height(inHeight)
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

	uint32 GetLayerCount() const { return LayerCount; }

protected:

	RHITexture2DArray(uint32 inWidth, uint32 inHeight, uint32 inLayerCount)
		: RHITexture2D(inWidth, inHeight), LayerCount(inLayerCount)
	{
	}

protected:

	uint32 LayerCount;

};

class RHITexture3D : public RHITexture
{
public:

	virtual ~RHITexture3D() = default;

protected:

	RHITexture3D(uint32 inWidth, uint32 inHeight, uint32 inDepth)
		: Width(inWidth), Height(inHeight), Depth(inDepth)
	{
	}

protected:

	uint32 Width;

	uint32 Height;

	uint32 Depth;

};