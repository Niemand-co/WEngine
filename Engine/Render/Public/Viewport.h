#pragma once
#include "Render/Public/RenderTarget.h"
#include "RHI/Public/RHIViewport.h"

class WViewport : public WRenderTarget
{
public:

	WViewport(uint32 InWidth, uint32 InHeight, EFormat InFormat)
		: WRenderTarget(InWidth, InHeight, InFormat),
		  Width(InWidth),
		  Height(InHeight),
		  PixelFormat(InFormat)
	{
		ViewportRHI = GetRenderCommandList()->CreateViewport(Width, Height, false, InFormat);
	}

	void AcquireImageIndex(uint32 ImageIndex) { AcquiredImageIndex = ImageIndex; }

	virtual ~WViewport() = default;

	virtual void BeginRenderFrame();

	virtual void ProcessInput() = 0;

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	virtual WTextureRHIRef GetHandle() const override { return ViewportRHI->GetRenderTarget(); }

	WViewportRHIRef GetRHI() const { return ViewportRHI; }

protected:

	uint32 Width;

	uint32 Height;

	EFormat PixelFormat;

	uint32 AcquiredImageIndex = 0;

	WViewportRHIRef ViewportRHI;

};

class WEditorViewport : public WViewport
{
public:

	WEditorViewport(uint32 width, uint32 height, EFormat format);

	virtual ~WEditorViewport();

	virtual void ProcessInput() override;

private:

	uint8 m_bActive : 1;

	uint8 m_bHovered : 1;

	uint8 m_bClicked : 1;

};

class WSceneViewport : public WViewport
{
public:

	WSceneViewport(uint32 width, uint32 height, EFormat format);

	virtual ~WSceneViewport();

	virtual void ProcessInput() override;

private:

	

};