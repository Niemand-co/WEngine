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



protected:

	unsigned int m_width;

	unsigned int m_height;

};