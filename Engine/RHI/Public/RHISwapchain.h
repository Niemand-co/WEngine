#pragma once

class RHITexture;

class RHISwapchain
{
public:

	virtual ~RHISwapchain() = default;

	virtual RHITexture* GetTexture(unsigned int index);

protected:

	WEngine::WArray<RHITexture*> m_textures;

};