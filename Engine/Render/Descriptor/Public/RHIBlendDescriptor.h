#pragma once

class RHIBlendDescriptor
{
public:

	RHIBlendDescriptor();

	~RHIBlendDescriptor() = default;

public:

	bool blendEnabled = true;

	BlendFactor colorSrcFactor = BlendFactor::FactorSrcAlpha;

	BlendFactor colorDstFactor = BlendFactor::FactorOneMinusSrcAlpha;

	BlendOP colorBlendOP = BlendOP::BlendAdd;

	BlendFactor alphaSrcFactor = BlendFactor::FactorZero;

	BlendFactor alphaDstFactor = BlendFactor::FactorOne;

	BlendOP alphaBlendOP = BlendOP::BlendAdd;

};