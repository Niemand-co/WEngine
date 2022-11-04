#pragma once

class RHIBuffer;
class RHITexture;

struct BufferBarrier
{
	RHIBuffer* pBuffer;
};

struct TextureBarrier
{
	RHITexture* pTexture;
	AttachmentLayout oldLayout;
	AttachmentLayout newLayout;
	unsigned int srcAccess;
	unsigned int dstAccess;
	unsigned int imageAspect;
};

class RHIBarrierDescriptor
{
public:

	RHIBarrierDescriptor();

	~RHIBarrierDescriptor() = default;

public:

	unsigned int srcStage;

	unsigned int dstStage;

	unsigned int bufferCount;

	BufferBarrier *pBufferBarriers;

	unsigned int textureCount;

	TextureBarrier *pTextureBarriers;

};