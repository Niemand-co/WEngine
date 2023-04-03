#pragma once

class RHIBlendDescriptor
{
public:

	RHIBlendDescriptor();

	~RHIBlendDescriptor() = default;

public:

	bool blendEnabled = true;

	EBlendFactor colorSrcFactor = EBlendFactor::FactorSrcAlpha;

	EBlendFactor colorDstFactor = EBlendFactor::FactorOneMinusSrcAlpha;

	EBlendOP colorBlendOP = EBlendOP::BlendAdd;

	EBlendFactor alphaSrcFactor = EBlendFactor::FactorZero;

	EBlendFactor alphaDstFactor = EBlendFactor::FactorOne;

	EBlendOP alphaBlendOP = EBlendOP::BlendAdd;

};