#pragma once

class RHIRenderPass;
class RHIRenderTarget;

class RHIRenderPassBeginDescriptor
{
public:

	RHIRenderPassBeginDescriptor();

	virtual ~RHIRenderPassBeginDescriptor();

public:

	RHIRenderPass *renderPass;

	RHIRenderTarget *renderTarget;

	Vector4 clearColor;

};