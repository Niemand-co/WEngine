#include "pch.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHITexture.h"

RHITexture* RHISwapchain::GetTexture(unsigned int index)
{
    return m_textures[index];
}