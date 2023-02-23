#pragma once

class RHIGraphicsEncoder;
class RHIComputeEncoder;
class RHIRenderPass;
class RHIRenderTarget;

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

	virtual void BeginPass(class RHIRenderPassBeginDescriptor* descriptor) = 0;

	virtual void SetPipeline(class RHIPipelineStateObject* pso) = 0;

	virtual void SetViewport(const Viewport& viewport) = 0;

	virtual void SetScissor(const Scissor& scissor) = 0;

	virtual void SetLineWidth(float width) = 0;

	virtual void SetDepthTestEnable(bool enable) = 0;

	virtual void SetPolygonTopology(PrimitiveTopology primitive) = 0;

	virtual void ClearRenderTarget(bool isClearColor, bool isClearDepth, glm::vec4 clearColor, float clearDepth = 0.0f, unsigned int clearStencil = 1) = 0;

	virtual void BindVertexBuffer(class RHIBuffer* pBuffer) = 0;

	virtual void BindIndexBuffer(class RHIBuffer* pBuffer) = 0;

	virtual void BindGroups(unsigned int groupCount, class RHIGroup* pGroup, class RHIPipelineResourceLayout* pPipelineResourceLayout, unsigned int offsetCount = 0, unsigned int* offsets = nullptr) = 0;

	virtual void DrawVertexArray() = 0;

	virtual void DrawIndexed(unsigned int indexeCount, unsigned int firstIndex, unsigned int instanceCount = 1, unsigned int firstInstance = 0) = 0;

	virtual void ResourceBarrier(unsigned int srcStage, unsigned int dstStage) = 0;

	virtual void ResourceBarrier(class RHIBarrierDescriptor* pDescriptor) = 0;

	virtual void CopyBufferToImage(class RHITexture* pTexture, RHIBuffer* pBuffer, unsigned int width, unsigned int height) = 0;

	virtual void SetEvent(class RHIEvent* pEvent) = 0;

	virtual void NextSubpass() = 0;

	virtual void EndPass() = 0;

	virtual void Clear() = 0;

};