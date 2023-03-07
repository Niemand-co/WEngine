#pragma once

class RHIPipelineStateObject
{
public:

	virtual ~RHIPipelineStateObject() = default;

	virtual void Bind(class RHICommandBuffer* CmdBuffer) = 0;

};