#pragma once

class RHIView : public RHIResource
{
public:

	virtual ~RHIView() = default;

};

class RHITextureView : public RHIView
{
public:

	virtual ~RHITextureView() = default;

protected:

	struct TextureSubresource
	{
		static constexpr uint32 kAllResource = WEngine::NumericLimits<uint32>::Max();

		uint32 MipLevel = kAllResource;
		uint32 MipCount = kAllResource;
		uint32 ArrayLayer = kAllResource;
		uint32 ArrayCount = kAllResource;
		uint32 PlaneSlice = kAllResource;
	};

};

class RHIBufferView : public RHIView
{

};