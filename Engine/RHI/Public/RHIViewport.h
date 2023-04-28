#pragma once

class RHICommandBuffer;
class RHIQueue;

class RHIViewport : public RHIResource
{
public:

	RHIViewport()
		: RHIResource(ERHIResourceType::RRT_Viewport)
	{
	}

	virtual ~RHIViewport() = default;

	virtual void Tick() = 0;

	virtual bool AcquireImageIndex() = 0;

	virtual int32 Present(RHICommandBuffer *CmdBuffer, RHIQueue* Queue) = 0;

	virtual WTextureRHIRef GetRenderTarget() const = 0;

};