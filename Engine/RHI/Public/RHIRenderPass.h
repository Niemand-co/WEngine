#pragma once

class RHIRenderPass
{
public:

	virtual ~RHIRenderPass() = default;

};

class RHIRenderPassManager
{
public:

	static WRenderPassRHIRef GetRenderPass(uint32 ID)
	{
		if (RenderPasses.Find(ID))
		{
			return RenderPasses[ID];
		}
		else
		{
			return nullptr;
		}
	}
	
	static void AddRenderPass(uint32 ID, WRenderPassRHIRef RenderPass)
	{
		RenderPasses.Insert(ID, RenderPass);
	}

public:

	static WEngine::WHashMap<uint32, WRenderPassRHIRef> RenderPasses;

};