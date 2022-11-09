#pragma once

class RHIGraphicsEncoder;
class RHIComputeEncoder;
class RHIRenderPass;
class RHIRenderTarget;

class RHICommandBuffer
{
public:

	virtual ~RHICommandBuffer() = default;

	virtual void BeginScopePass(std::string passName, RHIRenderPass* pRenderPass = nullptr, unsigned int subpass = 0, RHIRenderTarget* pRenderTarget = nullptr) = 0;

	virtual void EndScopePass() = 0;

	virtual void ExecuteCommandBuffer(RHICommandBuffer *pCommandBuffer) = 0;

	virtual RHIGraphicsEncoder* GetGraphicsEncoder() = 0;

	virtual RHIComputeEncoder* GetComputeEncoder() = 0;

	virtual void Clear() = 0;

};