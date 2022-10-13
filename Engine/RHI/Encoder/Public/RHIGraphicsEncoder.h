#pragma once
#include "RHI/Encoder/Public/RHIEncoder.h"
#include <span>

class RHIRenderPass;
class RHIPipelineStateObject;
class RHIViewport;
class RHIScissor;
class RHIBuffer;
class RHIGroup;
class RHIPipelineResourceLayout;
class RHITexture;
class RHIEvent;

class RHIRenderPassBeginDescriptor;

class RHIGraphicsEncoder : public RHIEncoder
{
public:

	virtual ~RHIGraphicsEncoder() = default;

	virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) = 0;

	virtual void SetPipeline(RHIPipelineStateObject *pso) = 0;

	virtual void SetViewport(RHIViewport *viewports) = 0;

	virtual void SetScissor(RHIScissor* scissor) = 0;

	virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth = 0.0f) = 0;

	virtual void BindVertexBuffer(RHIBuffer *pBuffer) = 0;

	virtual void BindIndexBuffer(RHIBuffer *pBuffer) = 0;

	virtual void BindGroups(unsigned int groupCount, RHIGroup *pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout) = 0;

	virtual void DrawVertexArray() = 0;

	virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) = 0;

	virtual void ResourceBarrier(RHITexture *pTexture) = 0;

	virtual void ResourceBarrier(RHIBuffer *pBuffer, size_t size) = 0;

	virtual void SetEvent(RHIEvent *pEvent) = 0;

	virtual void NextSubpass() = 0;

	virtual void EndPass() = 0;

};