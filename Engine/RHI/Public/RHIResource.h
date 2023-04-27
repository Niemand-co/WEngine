#pragma once
#include "Utils/Allocator/Public/Allocator.h"

enum ERHIResourceType : uint8
{
	RRT_None,
	RRT_VertexShader,
	RRT_PixelShader,
	RRT_GeometryShader,
	RRT_CommandBuffer,
	RRT_Texture,
	RRT_Buffer,
	RRT_BlendState,
	RRT_RasterizationState,
	RRT_DepthStencilState,
	RRT_MultiSampleState,
	RRT_RenderPass,
	RRT_GraphicsPipelineState,
	RRT_ComputePipelineState,
	RRT_UnorderedAccessView,
	RRT_ShaderResourceView,
	RRT_SamplerState,
	RRT_Barrier,
	RRT_Subresource,
	RRT_Device,
	RRT_State,
	RRT_PSO,
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
	uint8 ColorWirteMask = 0;
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

struct RHISamplerStateInitializer
{
	EFilter Filter = EFilter::FL_Linear;
	ESamplerAddressMode ModeU = ESamplerAddressMode::AM_Clamp;
	ESamplerAddressMode ModeV = ESamplerAddressMode::AM_Clamp;
	ESamplerAddressMode ModeW = ESamplerAddressMode::AM_Clamp;
	int32 MipBias = 0;
	int32 MaxAnisotropy = 1;
	uint32 BorderColor = 0;
};

struct RHIBoundShaderStateInput
{
	RHIBoundShaderStateInput() = default;

	RHIBoundShaderStateInput
	(
		RHIVertexInputState* InVertexInputState,
		WVertexShaderRHIRef InVertexShaderRHI,
		WPixelShaderRHIRef InPixelShaderRHI,
		WGeometryShaderRHIRef InGeometryShaderRHI
	)
		: VertexInputState(InVertexInputState),
		  VertexShaderRHI(InVertexShaderRHI),
		  PixelShaderRHI(InPixelShaderRHI),
		  GeometryShaderRHI(InGeometryShaderRHI)
	{
	}

	RHIVertexInputState *VertexInputState = nullptr;
	WVertexShaderRHIRef VertexShaderRHI = nullptr;
	WPixelShaderRHIRef PixelShaderRHI = nullptr;
	WGeometryShaderRHIRef GeometryShaderRHI = nullptr;
};

struct RHIGraphicsPipelineStateInitializer
{
public:

	using TRenderTargetFormats = WEngine::WStaticArray<uint8 /* EFormat */, MaxSimultaneousRenderTargets>;
	using TRenderTargetFlags = WEngine::WStaticArray<uint16 /* ETextureCreateFlags */, MaxSimultaneousRenderTargets>;
	using TRenderTargetLoadOps = WEngine::WStaticArray<uint8 /* EAttachmentLoadOp */, MaxSimultaneousRenderTargets>;
	using TRenderTargetStoreOps = WEngine::WStaticArray<uint8 /* EAttachmentStoreOp */, MaxSimultaneousRenderTargets>;
	using TRenderTargetInitialLayouts = WEngine::WStaticArray<uint8 /* EAttachmentLayout */, MaxSimultaneousRenderTargets>;
	using TImmutableSamplerStates = WEngine::WStaticArray<RHISamplerState*, 2>;

	RHIGraphicsPipelineStateInitializer()
		: BlendState(nullptr),
		  DepthStencilState(nullptr),
		  RasterizationState(nullptr),
		  MultiSampleState(nullptr),
		  ImmutableSamplers(nullptr),
		  PrimitiveType(EPrimitiveTopology::TriangleList),
		  RenderTargetEnabled(0),
		  RenderTargetFormats(EFormat::Unknown),
		  RenderTargetFlags(ETextureCreateFlags::TextureCreate_None),
		  RenderTargetLoadOps(EAttachmentLoadOP::Load),
		  RenderTargetStoreOps(EAttachmentStoreOP::DontCare),
		  RenderTargetInitialLayouts(EAttachmentLayout::Undefined),
		  DepthStencilFormat(EFormat::Unknown),
		  DepthTargetLoadAction(EAttachmentLoadOP::DontCare),
		  DepthTargetStoreAction(EAttachmentStoreOP::DontCare),
		  StencilTargetLoadAction(EAttachmentLoadOP::DontCare),
		  StencilTargetStoreAction(EAttachmentStoreOP::DontCare),
		  SubpassIndex(0),
		  NumSamples(0)
	{
	}

	RHIGraphicsPipelineStateInitializer
	(
		RHIBoundShaderStateInput    InBoundShaderState,
		RHIBlendState*              InBlendState,
		RHIDepthStencilState*       InDepthStencilState,
		RHIRasterizationState*      InRasterizationState,
		RHIMultiSampleState*        InMultiSampleState,
		TImmutableSamplerStates     InImmutableSamplers,
		EPrimitiveTopology		    InPrimitiveType,
		uint32					    InRenderTargetEnabled,
		TRenderTargetFormats	    InRenderTargetFormats,
		TRenderTargetFlags		    InRenderTargetFlags,
		TRenderTargetLoadOps        InRenderTargetLoadOps,
		TRenderTargetStoreOps       InRenderTargetStoreOps,
		TRenderTargetInitialLayouts InRenderTargetInitialLayouts,
		EFormat                     InDepthStencilFormat,
		EAttachmentLoadOP		    InDepthTargetLoadAction,
		EAttachmentStoreOP		    InDepthTargetStoreAction,
		EAttachmentLoadOP		    InStencilTargetLoadAction,
		EAttachmentStoreOP		    InStencilTargetStoreAction,
		uint8					    InSubpassIndex,
		uint16                      InNumSamples
	)
		: BoundShaderState(InBoundShaderState),
		  BlendState(InBlendState),
		  DepthStencilState(InDepthStencilState),
		  RasterizationState(InRasterizationState),
		  MultiSampleState(InMultiSampleState),
		  ImmutableSamplers(InImmutableSamplers),
		  PrimitiveType(InPrimitiveType),
		  RenderTargetEnabled(InRenderTargetEnabled),
		  RenderTargetFormats(InRenderTargetFormats),
		  RenderTargetFlags(InRenderTargetFlags),
		  RenderTargetLoadOps(InRenderTargetLoadOps),
		  RenderTargetStoreOps(InRenderTargetStoreOps),
		  RenderTargetInitialLayouts(InRenderTargetInitialLayouts),
		  DepthStencilFormat(InDepthStencilFormat),
		  DepthTargetLoadAction(InDepthTargetLoadAction),
		  DepthTargetStoreAction(InDepthTargetStoreAction),
		  StencilTargetLoadAction(InStencilTargetLoadAction),
		  StencilTargetStoreAction(InStencilTargetStoreAction),
		  SubpassIndex(InSubpassIndex),
		  NumSamples(InNumSamples)
	{
	}

	bool operator==(const RHIGraphicsPipelineStateInitializer& Other)
	{
		return BlendState == Other.BlendState                                 &&
			   DepthStencilState == Other.DepthStencilState                   &&
			   RasterizationState == Other.RasterizationState                 &&
			   MultiSampleState == Other.MultiSampleState                     &&
			   ImmutableSamplers == Other.ImmutableSamplers                   &&
			   PrimitiveType == Other.PrimitiveType                           &&
			   RenderTargetEnabled == Other.RenderTargetEnabled               &&
			   RenderTargetFormats == Other.RenderTargetFormats               &&
			   RenderTargetFlags == Other.RenderTargetFlags				      &&
			   RenderTargetLoadOps == Other.RenderTargetLoadOps               &&
			   RenderTargetStoreOps == Other.RenderTargetStoreOps             &&
			   RenderTargetInitialLayouts == Other.RenderTargetInitialLayouts &&
			   DepthTargetLoadAction == Other.DepthTargetLoadAction           &&
			   DepthTargetStoreAction == Other.DepthTargetStoreAction         &&
			   StencilTargetLoadAction == Other.StencilTargetLoadAction       &&
			   StencilTargetStoreAction == Other.StencilTargetStoreAction     &&
			   SubpassIndex == Other.SubpassIndex                             &&
			   NumSamples == Other.NumSamples;
	}

	uint32 GetValidRenderTargetNum()
	{
		for (uint32 Index = 0; Index < MaxSimultaneousRenderTargets; ++Index)
		{
			if (RenderTargetFormats[Index] == (uint8)EFormat::Unknown)
			{
				RenderTargetEnabled = Index;
				return Index;
			}
		}
		RenderTargetEnabled = MaxSimultaneousRenderTargets;
		return MaxSimultaneousRenderTargets;
	}

public:

	RHIBoundShaderStateInput    BoundShaderState;
	RHIBlendState*              BlendState;
	RHIDepthStencilState*       DepthStencilState;
	RHIRasterizationState*      RasterizationState;
	RHIMultiSampleState*        MultiSampleState;
	TImmutableSamplerStates     ImmutableSamplers;

	EPrimitiveTopology		    PrimitiveType;
	uint32					    RenderTargetEnabled;
	TRenderTargetFormats	    RenderTargetFormats;
	TRenderTargetLoadOps        RenderTargetLoadOps;
	TRenderTargetStoreOps       RenderTargetStoreOps;
	TRenderTargetInitialLayouts RenderTargetInitialLayouts;
	TRenderTargetFlags		    RenderTargetFlags;
	EFormat                     DepthStencilFormat;
	EAttachmentLoadOP		    DepthTargetLoadAction;
	EAttachmentStoreOP		    DepthTargetStoreAction;
	EAttachmentLoadOP		    StencilTargetLoadAction;
	EAttachmentStoreOP		    StencilTargetStoreAction;
							    
	uint8					    SubpassIndex;
	uint16					    NumSamples;
};