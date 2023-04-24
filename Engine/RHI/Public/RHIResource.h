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

struct RHIBlendStateInitializer
{
	bool bEnableBlend = true;
	EBlendOP ColorBlendOp = EBlendOP::BlendAdd;
	EBlendFactor ColorSrcFactor = EBlendFactor::FactorOne;
	EBlendFactor ColorDstFactor = EBlendFactor::FactorZero;
	EBlendOP AlphaBlendOp = EBlendOP::BlendAdd;
	EBlendFactor AlphaSrcFactor = EBlendFactor::FactorOne;
	EBlendFactor AlphaDstFactor = EBlendFactor::FactorZero;
};

struct RHIDepthStencilStateInitializer
{
	bool bEnableDepthTest = true;
	bool bEnableDepthWrite = true;
	ECompareOP DepthCompareOp = ECompareOP::NotEqual;
	bool bEnableFrontStencil = false;
	ECompareOP FrontStencilCompareOp = ECompareOP::Always;
	EStencilFailedOP FrontStencilFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP FrontDepthFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP FrontStencilPassOp = EStencilFailedOP::Replace;
	bool bEnableBackStencil = false;
	ECompareOP BackStencilCompareOp = ECompareOP::Always;
	EStencilFailedOP BackStencilFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP BackDepthFailedOp = EStencilFailedOP::Keep;
	EStencilFailedOP BackStencilPassOp = EStencilFailedOP::Replace;
	uint8 StencilReadMask = 0xFF;
	uint8 StencilWriteMask = 0xFF;
};

struct RHIRasterizationStateInitializer
{
	ECullMode CullMode = ECullMode::None;
	EPolygonMode PolygonMode = EPolygonMode::Triangle;
	EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::TriangleList;
	bool bEnableDepthClamp = false;
	bool bEnableDiscard = false;
	bool bClockWise = false;
};

struct RHIMultiSampleStateInitializer
{
	bool bEnableSampleShading = false;
	uint32 SampleCount = 1;
	uint32 SampleMask = 0x00;
	float MinSampleShading = 0.f;
	bool bEnableAlphaToCoverage = false;
	bool bEnableAlphaToOne = false;
};

struct RHIGraphicsPipelineStateInitializer
{
public:

	RHIGraphicsPipelineStateInitializer()
		:
	{
	}

public:

	RHIBlendState *BlendState;
	RHIDepthStencilState *DepthStencilState;
	RHIRasterizationState *RasterizationState;
	RHIMultiSampleState *MultiSampleState;

	EPrimitiveTopology PrimitiveType;
};