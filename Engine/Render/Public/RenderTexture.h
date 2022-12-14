#pragma once
#include "Render/Public/RenderResource.h"

class RHITexture;
class RHITextureView;
class RHITextureDescriptor;
class RHITextureViewDescriptor;

class RenderTexture : public RenderResource
{
public:

	RenderTexture(unsigned int width, unsigned int height, Format format);

	RenderTexture(unsigned int width, unsigned int height, Format format, unsigned int mipLevel, unsigned int layerCount);

	virtual ~RenderTexture() = default;

	void CreateObject();

private:

	WEngine::WArray<RHITexture*> m_textures;

	WEngine::WArray<RHITextureView*> m_textureViews;

	RHITextureDescriptor m_textureDescriptor;

	RHITextureViewDescriptor m_textureViewDescriptor;

	/** Whether the texture has been created. */
	bool m_bCreated;

};