#pragma once

class RHIPipelineStateObject : public RHIResource
{
public:

	RHIPipelineStateObject()
		: RHIResource(ERHIResourceType::RRT_PSO)
	{
	}

	virtual ~RHIPipelineStateObject() = default;

	virtual void Bind(class RHICommandBuffer* CmdBuffer) = 0;

};

class RHIGraphicsPipelineState : public RHIPipelineStateObject
{
public:

	virtual ~RHIGraphicsPipelineState() = default;

};

class RHIComputePipelineState : public RHIPipelineStateObject
{
public:

	virtual ~RHIComputePipelineState() = default;

};