#pragma once

class RHIRenderPass;
class RHIRenderTarget;

class RHIRenderPassBeginDescriptor
{
public:

	RHIRenderPassBeginDescriptor();

	~RHIRenderPassBeginDescriptor() = default;

public:

	RHIRenderPass *renderPass;

	RHIRenderTarget *renderTarget;

	unsigned int clearCount;

	ClearValue *pClearValues;

};