#pragma once

template<typename StateType, typename StateRHIType>
class TStaticStateRHI
{
public:
	
	static StateRHIType* GetRHI()
	{
		return State.StaticRHI;
	}
	
public:

	static StateType State;

};

template<typename StateType, typename StateRHIType>
StateType TStaticStateRHI<StateType, StateRHIType>::State;

template<
	bool bEnableDepthTest = true,
	bool bEnableDepthWrite = true,
	ECompareOP DepthCompareOp = ECompareOP::NotEqual,
	bool bEnableFrontStencil = false,
	ECompareOP FrontStencilCompareOp = ECompareOP::Always,
	EStencilFailedOP FrontStencilFailedOp = EStencilFailedOP::Keep,
	EStencilFailedOP FrontDepthFailedOp = EStencilFailedOP::Keep,
	EStencilFailedOP FrontStencilPassOp = EStencilFailedOP::Replace,
	bool bEnableBackStencil = false,
	ECompareOP BackStencilCompareOp = ECompareOP::Always,
	EStencilFailedOP BackStencilFailedOp = EStencilFailedOP::Keep,
	EStencilFailedOP BackDepthFailedOp = EStencilFailedOP::Keep,
	EStencilFailedOP BackStencilPassOp = EStencilFailedOP::Replace,
	uint8 StencilReadMask = 0xFF,
	uint8 StencilWriteMask = 0xFF
>
class TStaticDepthStencilStateRHI : public TStaticStateRHI< TStaticDepthStencilStateRHI<
	bEnableDepthTest,
	bEnableDepthWrite,
	DepthCompareOp,
	bEnableFrontStencil,
	FrontStencilCompareOp,
	FrontStencilFailedOp,
	FrontDepthFailedOp,
	FrontStencilPassOp,
	bEnableBackStencil,
	BackStencilCompareOp,
	BackStencilFailedOp,
	BackDepthFailedOp,
	BackStencilPassOp,
	StencilReadMask,
	StencilWriteMask
	>, class RHIDepthStencilState>
{
public:

	TStaticDepthStencilStateRHI()
	{
		RHIDepthStencilStateInitializer Initializer =
		{
			bEnableDepthTest,
			bEnableDepthWrite,
			DepthCompareOp,
			bEnableFrontStencil,
			FrontStencilCompareOp,
			FrontStencilFailedOp,
			FrontDepthFailedOp,
			FrontStencilPassOp,
			bEnableBackStencil,
			BackStencilCompareOp,
			BackStencilFailedOp,
			BackDepthFailedOp,
			BackStencilPassOp,
			StencilReadMask,
			StencilWriteMask
		};
		StaticRHI = GetRenderCommandList()->CreateDepthStencilState(Initializer);
	}

public:

	WDepthStencilStateRHIRef StaticRHI;

};

template<
	bool bEnableBlend = true,
	EBlendOP ColorBlendOp = EBlendOP::BlendAdd,
	EBlendFactor ColorSrcFactor = EBlendFactor::FactorOne,
	EBlendFactor ColorDstFactor = EBlendFactor::FactorZero,
	EBlendOP AlphaBlendOp = EBlendOP::BlendAdd,
	EBlendFactor AlphaSrcFactor = EBlendFactor::FactorOne,
	EBlendFactor AlphaDstFactor = EBlendFactor::FactorZero
>
class TStaticBlendStateRHI : public TStaticStateRHI<TStaticBlendStateRHI<
	bEnableBlend,
	ColorBlendOp,
	ColorSrcFactor,
	ColorDstFactor,
	AlphaBlendOp,
	AlphaSrcFactor,
	AlphaDstFactor
	>, class RHIBlendState>
{
public:

	TStaticBlendStateRHI()
	{
		RHIBlendStateInitializer Initializer = 
		{
			bEnableBlend,
			ColorBlendOp,
			ColorSrcFactor,
			ColorDstFactor,
			AlphaBlendOp,
			AlphaSrcFactor,
			AlphaDstFactor
		};
		StaticRHI = GetRenderCommandList()->CreateBlendState(Initializer);
	}

public:

	WBlendStateRHIRef StaticRHI;

};

template<
	ECullMode CullMode = ECullMode::None,
	EPolygonMode PolygonMode = EPolygonMode::Triangle,
	EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::TriangleList,
	bool bEnableDepthClamp = false,
	bool bEnableDiscard = false,
	bool bClockWise = false
>
class TStaticRasterizationStateRHI : public TStaticStateRHI<TStaticRasterizationStateRHI<
	CullMode,
	PolygonMode,
	PrimitiveTopology,
	bEnableDepthClamp,
	bEnableDiscard,
	bClockWise
	>, class RHIRasterizationState>
{
public:

	TStaticRasterizationStateRHI()
	{
		RHIRasterizationStateInitializer Initializer =
		{
			CullMode,
			PolygonMode,
			PrimitiveTopology,
			bEnableDepthClamp,
			bEnableDiscard,
			bClockWise
		};
		StaticRHI = GetRenderCommandList()->CreateRasterizationState(Initializer);
	}

public:

	WRasterizationStateRHIRef StaticRHI;

};

template<
	bool bEnableSampleShading = false,
	uint32 SampleCount = 1,
	uint32 SampleMask = 0x00,
	float MinSampleShading = 0.f,
	bool bEnableAlphaToCoverage = false,
	bool bEnableAlphaToOne = false
>
class TStaticMultiSampleStateRHI : public TStaticStateRHI<TStaticMultiSampleStateRHI<
	bEnableSampleShading,
	SampleCount,
	SampleMask,
	MinSampleShading,
	bEnableAlphaToCoverage,
	bEnableAlphaToOne
>, class RHIMultiSampleState>
{
public:

	TStaticMultiSampleStateRHI()
	{
		RHIMultiSampleStateInitializer Initializer = 
		{
			bEnableSampleShading,
			SampleCount,
			SampleMask,
			MinSampleShading,
			bEnableAlphaToCoverage,
			bEnableAlphaToOne
		};
		StaticRHI = GetRenderCommandList()->CreateMultiSampleState(Initializer);
	}

public:

	WMultiSampleStateRHIRef StaticRHI;

};