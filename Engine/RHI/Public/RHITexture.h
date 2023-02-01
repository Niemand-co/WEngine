#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;
class RHIContext;

class RHITexture : public RHIResource
{
public:

	virtual ~RHITexture() = default;

	virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor* descriptor) = 0;

	virtual void LoadData(const WEngine::WString& path, RHIContext *context) = 0;

};

class RHITexture2D : public RHITexture
{
public:

	virtual ~RHITexture2D() = default;

	uint32 GetWidth() const { return Width; }

	uint32 GetHeight() const { return Height; }

protected:

	uint32 Width;

	uint32 Height;

};

class RHITexture3D : public RHITexture
{
public:

	virtual ~RHITexture3D() = default;

};

class RHITexture2DArray : public RHITexture2D
{
public:

	virtual ~RHITexture2DArray() = default;

	uint32 GetDepth() const { return Depth; }

protected:

	uint32 Depth;

};

class RHITexture3DArray : public RHITexture
{
public:

	virtual ~RHITexture3DArray() = default;

};