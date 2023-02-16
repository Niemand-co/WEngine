#pragma once
#include "Render/Public/RenderDependencyGraphDefinitions.h"

class RHIBuffer;
class RHITexture;

struct BufferBarrier
{
	RHIBuffer* pBuffer;
};

struct RHISubresource
{
public:

	static constexpr uint32 kDepthPlaneSlice = 0x0;
	static constexpr uint32 kStencilPlaneSlice = 0x1;

	static constexpr uint32 kAllResource = WEngine::NumericLimits<uint32>::Max();

	RHISubresource() = default;

	RHISubresource(uint32 inMipLevel, uint32 inArrayLayer, uint32 inPlaneSlice)
		: MipLevel(inMipLevel),
		  ArrayLayer(inArrayLayer),
  		  PlaneSlice(inPlaneSlice)
	{
	}

	~RHISubresource() = default;

	bool IsWholeMipLevel() const { return MipLevel == kAllResource; }

	bool IsWholeArrayLayer() const { return ArrayLayer == kAllResource; }

	bool IsWholePlaneSlice() const { return PlaneSlice == kAllResource; }

	bool IsWholeResource() const { return MipLevel == kAllResource && ArrayLayer == kAllResource && PlaneSlice == kAllResource; }

	bool IgnoreDepthSlice() const { return PlaneSlice == kStencilPlaneSlice; }

	bool IgnoreStencilSlice() const { return PlaneSlice == kDepthPlaneSlice; }

public:

	uint32 MipLevel = kAllResource;

	uint32 ArrayLayer = kAllResource;

	uint32 PlaneSlice = kAllResource;

};

class RHIBarrierDescriptor : public RHISubresource
{
public:

	RHIBarrierDescriptor() = default;

	RHIBarrierDescriptor(class RHITexture* inTexture, EAccess inAccessBefore, EAccess inAccessAfter, uint32 inMipLevel, uint32 inArrayLayer, uint32 inPlaneSlice)
		: Texture(inTexture),
		  Type(EType::Texture),
		  AccessBefore(inAccessBefore),
		  AccessAfter(inAccessAfter),
		  RHISubresource(inMipLevel, inArrayLayer, inPlaneSlice)
	{
	}

	RHIBarrierDescriptor(class RHIBuffer* inBuffer, EAccess inAccessBefore, EAccess inAccessAfter, uint32 inMipLevel, uint32 inArrayLayer, uint32 inPlaneSlice)
		: Buffer(inBuffer),
		  Type(EType::Buffer),
		  AccessBefore(inAccessBefore),
		  AccessAfter(inAccessAfter),
		  RHISubresource(inMipLevel, inArrayLayer, inPlaneSlice)
	{
	}


	~RHIBarrierDescriptor() = default;

public:

	enum class EType : uint8
	{
		Unknown,
		Texture,
		Buffer,
	};

	EAccess AccessBefore = EAccess::Unknown;

	EAccess AccessAfter = EAccess::Unknown;

	EType Type;

	union
	{
		class RHIResource *Resource;
		class RHITexture* Texture;
		class RHIBuffer* Buffer;
	};

};