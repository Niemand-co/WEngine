#include "pch.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"

RHIBlendDescriptor::RHIBlendDescriptor()
{
	alphaBlendOP = BlendOP::BlendAdd;
	alphaSrcFactor = BlendFactor::FactorZero;
	alphaDstFactor = BlendFactor::FactorZero;
	colorBlendOP = BlendOP::BlendAdd;
	colorSrcFactor = BlendFactor::FactorZero;
	colorDstFactor = BlendFactor::FactorZero;
}