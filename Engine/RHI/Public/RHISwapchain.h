#pragma once

class RHITexture;

class RHISwapchain
{
public:

	virtual ~RHISwapchain() = default;

	virtual RHITexture* GetTexture(unsigned int index);

protected:

	std::vector<RHITexture*> m_textures;

};