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

	/**
	 * Resize the texture
	 * 
	 * @param width
	 * @param height
	 */
	void Resize(unsigned int width, unsigned int height);

	inline unsigned int GetWidth() const { return m_width; }

	inline unsigned int GetHeight() const { return m_height; }

private:

	WEngine::WArray<RHITexture*> m_textures;

	WEngine::WArray<RHITextureView*> m_textureViews;

	RHITextureDescriptor m_textureDescriptor;

	RHITextureViewDescriptor m_textureViewDescriptor;

	AttachmentLayout m_layout;

	/** Whether the texture has been created. */
	bool m_bCreated;

	/** The width of the texture */
	unsigned int m_width;

	/** The height of the texture */
	unsigned int m_height;

};