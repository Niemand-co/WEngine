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
};

class RHIBarrierDescriptor
{
public:

	RHIBarrierDescriptor();

	~RHIBarrierDescriptor();

public:

	unsigned int srcStage;

	unsigned int dstStage;

	unsigned int bufferCount;

	BufferBarrier *pBufferBarriers;

	unsigned int textureCount;

	TextureBarrier *pTextureBarriers;

};