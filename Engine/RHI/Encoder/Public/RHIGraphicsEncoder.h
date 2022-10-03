#pragma once
#include "RHI/Encoder/Public/RHIEncoder.h"
#include <span>

class RHIRenderPass;
class RHIPipelineStateObject;
class RHIViewport;
class RHIScissor;
class RHIBuffer;

class RHIRenderPassBeginDescriptor;

class RHIGraphicsEncoder : public RHIEncoder
{
public:

	virtual ~RHIGraphicsEncoder() = default;

	virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) = 0;

	virtual void SetPipeline(RHIPipelineStateObject *pso) = 0;

	virtual void SetViewport(RHIViewport *viewports) = 0;

	virtual void SetScissor(RHIScissor* scissor) = 0;

	virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, Vector4 clearColor, float clearDepth = 0.0f) = 0;

	virtual void BindVertexBuffer(RHIBuffer *pBuffer) = 0;

	virtual void DrawVertexArray() = 0;

	virtual void EndPass() = 0;

};