#pragma once

class RHIFramebuffer : public RHIResource
{
public:

	virtual ~RHIFramebuffer() = default;

};

class RHIFramebufferManager
{
public:

	static WFramebufferRHIRef GetFramebuffer(uint32 ID)
	{
		if (Framebuffers.Find(ID))
		{
			return Framebuffers[ID];
		}
		return nullptr;
	}

	static void AddFramebuffer(uint32 ID, WFramebufferRHIRef Framebuffer)
	{
		Framebuffers.Insert(ID, Framebuffer);
	}

public:

	static WEngine::WHashMap<uint32, WFramebufferRHIRef> Framebuffers;

};