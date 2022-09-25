#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIRenderPass;
class RHIRenderTarget;

class RHIRenderPassBeginDescriptor : public RHIDescriptor
{
public:

	RHIRenderPassBeginDescriptor();

	virtual ~RHIRenderPassBeginDescriptor();

public:

	RHIRenderPass *renderPass;

	RHIRenderTarget *renderTarget;

	Vector4 clearColor;

};