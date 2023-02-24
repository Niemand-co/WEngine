#pragma once

class RHIGraphicsEncoder;
class RHIComputeEncoder;
class RHIRenderPass;
class RHIRenderTarget;
class RHIPipelineStateObject;
class RHIBuffer;
class RHITexture;
class RHIEvent;
class RHIPipelineResourceLayout;
class RHIGroup;

class RHIRenderPassBeginDescriptor;
class RHIBarrierDescriptor;

struct Viewport
{

};

struct Scissor
{

};

class RHICommandBuffer : public RHIResource
{
public:

	virtual ~RHICommandBuffer() = default;

	virtual void BeginScopePass(const WEngine::WString& passName) = 0;

	virtual void EndScopePass() = 0;

	virtual void ExecuteCommandBuffer(RHICommandBuffer *pCommandBuffer) = 0;

	virtual void BeginPass(RHIRenderPassBeginDescriptor* descriptor) = 0;

	virtual void SetPipeline(RHIPipelineStateObject* pso) = 0;

	virtual void SetViewport(const Viewport& viewport) = 0;

	virtual void SetScissor(const Scissor& scissor) = 0;

	virtual void SetLineWidth(float width) = 0;

	virtual void SetDepthTestEnable(bool enable) = 0;

	virtual void SetPolygonTopology(PrimitiveTopology primitive) = 0;

	virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth = 0.0f, unsigned int clearStencil = 1) = 0;

	virtual void BindVertexBuffer(RHIBuffer* pBuffer) = 0;

	virtual void BindIndexBuffer(RHIBuffer* pBuffer) = 0;

	virtual void BindGroups(unsigned int groupCount, RHIGroup* pGroup, RHIPipelineResourceLayout* pPipelineResourceLayout, unsigned int offsetCount = 0, unsigned int* offsets = nullptr) = 0;

	virtual void DrawVertexArray() = 0;

	virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) = 0;

	virtual void ResourceBarrier(unsigned int srcStage, unsigned int dstStage) = 0;

	virtual void ResourceBarrier(RHIBarrierDescriptor* pDescriptor) = 0;

	virtual void CopyBufferToImage(RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height) = 0;

	virtual void SetEvent(RHIEvent* pEvent) = 0;

	virtual void NextSubpass() = 0;

	virtual void EndPass() = 0;

	virtual void Clear() = 0;

};