#pragma once

class RHIGraphicsEncoder;
class RHIComputeEncoder;

class RHICommandBuffer
{
public:

	virtual ~RHICommandBuffer() = default;

	virtual void BeginScopePass(std::string passName) = 0;

	virtual void EndScopePass() = 0;

	virtual RHIGraphicsEncoder* GetGraphicsEncoder() = 0;

	virtual RHIComputeEncoder* GetComputeEncoder() = 0;

};