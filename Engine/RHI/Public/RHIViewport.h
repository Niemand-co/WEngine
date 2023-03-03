#pragma once

class RHICommandBuffer;
class RHIQueue;

class RHIViewport : public RHIResource
{
public:

	virtual ~RHIViewport() = default;

	virtual void Tick() = 0;

	virtual bool AcquireImageIndex() = 0;

	virtual int32 Present(RHICommandBuffer *CmdBuffer, RHIQueue* Queue) = 0;

	virtual RHITexture* GetRenderTarget() const = 0;

};