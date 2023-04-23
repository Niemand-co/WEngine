#pragma once
#include "Utils/Allocator/Public/Allocator.h"

enum ERHIResourceType : uint8
{
	RRT_None,
	RRT_VertexShader,
	RRT_PixelShader,
	RRT_GeometryShader,
	RRT_Texture,
	RRT_Buffer,
	RRT_BlendState,
	RRT_RasterizationState,
	RRT_DepthStencilState,
	RRT_MultiSampleState,
	RRT_GraphicsPipelineState,
	RRT_ComputePipelineState,
	RRT_UnorderedAccessView,
	RRT_ShaderResourceView,
	RRT_Barrier,
	RRT_Subresource,
};

class RHIResource
{
public:

	RHIResource(ERHIResourceType InType)
		: Type(InType)
	{
	}

	virtual ~RHIResource() = default;

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

protected:

	ERHIResourceType Type;

};

class RHIViewableResource : public RHIResource
{
public:

	virtual ~RHIViewableResource() = default;

protected:

	RHIViewableResource(ERHIResourceType InType, EAccess InAccess)
		: RHIResource(InType),
		  Access(InAccess)
	{
	}

private:

	EAccess Access;

};