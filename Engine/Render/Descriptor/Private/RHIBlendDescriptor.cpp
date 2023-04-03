#include "pch.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"

RHIBlendDescriptor::RHIBlendDescriptor()
{
	alphaBlendOP = EBlendOP::BlendAdd;
	alphaSrcFactor = EBlendFactor::FactorZero;
	alphaDstFactor = EBlendFactor::FactorZero;
	colorBlendOP = EBlendOP::BlendAdd;
	colorSrcFactor = EBlendFactor::FactorZero;
	colorDstFactor = EBlendFactor::FactorZero;
}