#pragma once

class RHIRenderPass;
class RHIRenderTarget;

struct ClearValue
{
	glm::vec4 value;
	bool color;
};

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