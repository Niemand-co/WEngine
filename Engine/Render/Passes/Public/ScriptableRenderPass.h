#pragma once

class ScriptableRenderPass : public RenderResource
{
public:

	ScriptableRenderPass() = default;

	virtual ~ScriptableRenderPass() = default;

protected:

	WRenderPassRHIRef RenderPass = nullptr;

protected:

	static WEngine::WHashMap<int32, WRenderPassRHIRef> Passes;

};
