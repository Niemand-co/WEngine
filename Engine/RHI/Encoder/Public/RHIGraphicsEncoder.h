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
class RHIBarrierDescriptor;

class RHIGraphicsEncoder : public RHIEncoder
{
public:

	virtual ~RHIGraphicsEncoder() = default;

	virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) = 0;

	virtual void SetPipeline(RHIPipelineStateObject *pso) = 0;

	virtual void SetViewport(const RHIViewport& viewports) = 0;

	virtual void SetScissor(const RHIScissor& scissor) = 0;

	virtual void SetLineWidth(float width) = 0;

	virtual void SetDepthTestEnable(bool enable) = 0;

	virtual void SetPolygonMode(PolygonMode mode) = 0;

	virtual void SetPolygonTopology(PrimitiveTopology primitive) = 0;

	virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth = 0.0f, unsigned int clearStencil = 1) = 0;

	virtual void BindVertexBuffer(RHIBuffer *pBuffer) = 0;

	virtual void BindIndexBuffer(RHIBuffer *pBuffer) = 0;

	virtual void BindGroups(unsigned int groupCount, RHIGroup *pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout, unsigned int offsetCount = 0, unsigned int* offsets = nullptr) = 0;

	virtual void DrawVertexArray() = 0;

	virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) = 0;

	virtual void ResourceBarrier(unsigned int srcStage, unsigned int dstStage) = 0;

	virtual void ResourceBarrier(RHIBarrierDescriptor *pDescriptor) = 0;

	virtual void CopyBufferToImage(RHITexture *pTexture, RHIBuffer *pBuffer, unsigned int width, unsigned int height) = 0;

	virtual void SetEvent(RHIEvent *pEvent) = 0;

	virtual void NextSubpass() = 0;

	virtual void EndPass() = 0;

protected:

	unsigned int m_width;

	unsigned int m_height;

};