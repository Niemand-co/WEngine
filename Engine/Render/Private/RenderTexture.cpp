#include "pch.h"
#include "Render/Public/RenderTexture.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHITextureView.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHITextureDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/Descriptor/Public/RHIBarrierDescriptor.h"

RenderTexture::RenderTexture(unsigned int width, unsigned int height, Format format)
{
	m_textureDescriptor.width = width;
	m_textureDescriptor.height = height;
	m_textureDescriptor.format = format;

	if(format == Format::D16_Unorm || format == Format::D32_SFloat)
	{
		m_textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
	}
	else if (format == Format::D16_UNORM_S8_UINT || format == Format::D24_UNORM_S8_UINT || format == Format::D32_SFLOAT_S8_UINT)
	{
		m_textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH | IMAGE_ASPECT_STENCIL;
	}
	else
	{
		m_textureDescriptor.usage = IMAGE_USAGE_COLOR_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
	}

	m_textureDescriptor.layerCount =  1;
	m_textureDescriptor.mipCount = 1;
	m_textureDescriptor.layout = AttachmentLayout::Undefined;

	m_textureViewDescriptor.format = format;
	m_textureViewDescriptor.arrayLayerCount = 1;
	m_textureViewDescriptor.mipCount = 1;
	m_textureViewDescriptor.baseArrayLayer = 0;
	m_textureViewDescriptor.baseMipLevel = 0;
	m_textureViewDescriptor.dimension = Dimension::Texture2D;
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, Format format, unsigned int mipLevel, unsigned int layerCount)
{
	m_textureDescriptor.width = width;
	m_textureDescriptor.height = height;
	m_textureDescriptor.format = format;

	if (format == Format::D16_Unorm || format == Format::D32_SFloat)
	{
		m_textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH;
		m_layout = AttachmentLayout::DepthBuffer;
	}
	else if (format == Format::D16_UNORM_S8_UINT || format == Format::D24_UNORM_S8_UINT || format == Format::D32_SFLOAT_S8_UINT)
	{
		m_textureDescriptor.usage = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_DEPTH | IMAGE_ASPECT_STENCIL;
		m_layout = AttachmentLayout::DepthBuffer;
	}
	else
	{
		m_textureDescriptor.usage = IMAGE_USAGE_COLOR_ATTACHMENT;
		m_textureViewDescriptor.imageAspect = IMAGE_ASPECT_COLOR;
		m_layout = AttachmentLayout::ColorBuffer;
	}

	m_textureDescriptor.layerCount = layerCount;
	m_textureDescriptor.mipCount = mipLevel;
	m_textureDescriptor.layout = AttachmentLayout::Undefined;

	m_textureViewDescriptor.format = format;
	m_textureViewDescriptor.arrayLayerCount = layerCount;
	m_textureViewDescriptor.mipCount = mipLevel;
	m_textureViewDescriptor.baseArrayLayer = 0;
	m_textureViewDescriptor.baseMipLevel = 0;
	m_textureViewDescriptor.dimension = Dimension::Texture2D;

	m_width = width;
	m_height = height;
}

void RenderTexture::CreateObject()
{
	if(m_bCreated)
		return;

	m_textures.Resize(RHIContext::g_maxFrames);
	m_textureViews.Resize(RHIContext::g_maxFrames);
	WEngine::WArray<TextureBarrier> barriers(RHIContext::g_maxFrames);
	unsigned int dstAccess = 0;
	if(m_textureViewDescriptor.imageAspect & IMAGE_ASPECT_COLOR > 0)
		dstAccess |= (ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE);
	if(m_textureViewDescriptor.imageAspect & IMAGE_ASPECT_DEPTH > 0)
		dstAccess |= (ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE);
	for (unsigned int i = 0; i < m_textures.Size(); ++i)
	{
		m_textures[i] = RHIContext::GetDevice()->CreateTexture(&m_textureDescriptor);
		barriers[i] = { m_textures[i], AttachmentLayout::Undefined, m_layout, 0, dstAccess, m_textureViewDescriptor.imageAspect };
	}

	RHIBarrierDescriptor barrierDescriptor = {};
	{
		barrierDescriptor.textureCount = barriers.Size();
		barrierDescriptor.pTextureBarriers = barriers.GetData();
		barrierDescriptor.srcStage = PIPELINE_STAGE_TOP_OF_PIPE;
		barrierDescriptor.dstStage = PIPELINE_STAGE_EARLY_FRAGMENT_TESTS;
	}
	RHIContext::GetContext()->ResourceBarrier(&barrierDescriptor);

	for (unsigned int i = 0; i < m_textureViews.Size(); ++i)
	{
		m_textureViews[i] = m_textures[i]->CreateTextureView(&m_textureViewDescriptor);
	}

	m_bCreated = true;
}

void RenderTexture::Resize(unsigned int width, unsigned int height)
{
	WEngine::WArray<TextureBarrier> barriers(RHIContext::g_maxFrames);

	for (unsigned int i = 0; i < m_textures.Size(); ++i)
	{
		delete m_textures[i];
		delete m_textureViews[i];
	}
	m_textureDescriptor.width = width;
	m_textureDescriptor.height = height;

	m_width = width;
	m_height = height;

	m_bCreated = false;
}
