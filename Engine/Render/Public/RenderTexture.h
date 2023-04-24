#pragma once
#include "Render/Public/RenderResource.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"

class RHITexture;
class RHITextureView;

class RenderTexture : public RenderResource
{
public:

	RenderTexture(unsigned int width, unsigned int height, EFormat format);

	RenderTexture(unsigned int width, unsigned int height, EFormat format, unsigned int mipLevel, unsigned int layerCount);

	virtual ~RenderTexture() = default;

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	/**
	 * Resize the texture
	 * 
	 * @param width
	 * @param height
	 */
	void Resize(unsigned int width, unsigned int height);

	inline unsigned int GetWidth() const { return m_width; }

	inline unsigned int GetHeight() const { return m_height; }

	inline void MakrDirty() { m_bDirty = true; }

protected:

	WEngine::WSharedPtr<RHITexture> m_texture;

	WEngine::WSharedPtr<RHITextureView> m_textureView;

	RHITextureDescriptor m_textureDescriptor;

	RHITextureViewDescriptor m_textureViewDescriptor;

	AttachmentLayout m_layout;

	/** Whether the texture has been created. */
	uint8 m_bCreated : 1;

	/** The width of the texture */
	unsigned int m_width;

	/** The height of the texture */
	unsigned int m_height;

	/** Whether the texture state has been changed */
	uint8 m_bDirty : 1;

};

class UAVTexture : public RenderTexture
{
public:

	UAVTexture(unsigned int width, unsigned int height, EFormat format);

	UAVTexture(unsigned int width, unsigned int height, EFormat format, unsigned int mipLevel, unsigned int layerCount);



};

class SRVTexture : public RenderTexture
{
public:

	SRVTexture(unsigned int width, unsigned int height, EFormat format);

	SRVTexture(unsigned int width, unsigned int height, EFormat format, unsigned int mipLevel, unsigned int layerCount);

};

class ShadowMap2D : public SRVTexture
{
public:

	ShadowMap2D(unsigned int width, unsigned int height);

	ShadowMap2D(unsigned int width, unsigned int height, EFormat format);

};