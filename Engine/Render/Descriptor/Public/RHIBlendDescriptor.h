#pragma 
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIBlendDescriptor : RHIDescriptor
{
public:

	RHIBlendDescriptor();

	virtual ~RHIBlendDescriptor();

public:

	bool blendEnabled;

	BlendFactor colorSrcFactor;

	BlendFactor colorDstFactor;

	BlendOP colorBlendOP;

	BlendFactor alphaSrcFactor;

	BlendFactor alphaDstFactor;

	BlendOP alphaBlendOP;

};