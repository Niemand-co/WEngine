#pragma once

class RHIView : public RHIResource
{
public:

	

};

class RHITextureView : public RHIView
{
public:

	virtual ~RHITextureView() = default;

private:

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