#pragma once

class RHIBlendDescriptor
{
public:

	RHIBlendDescriptor();

	~RHIBlendDescriptor() = default;

public:

	bool blendEnabled;

	BlendFactor colorSrcFactor;

	BlendFactor colorDstFactor;

	BlendOP colorBlendOP;

	BlendFactor alphaSrcFactor;

	BlendFactor alphaDstFactor;

	BlendOP alphaBlendOP;

};