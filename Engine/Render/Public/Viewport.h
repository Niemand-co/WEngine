#pragma once
#include "Render/Public/RenderResource.h"
#include "Render/Public/RenderTarget.h"

class WViewport : public RenderTarget
{
public:

	WViewport(uint32 width, uint32 height, Format format)
		: RenderTarget(width, height, format)
	{
	}

	virtual ~WViewport() = default;

	virtual void ProcessInput() = 0;

protected:

	glm::vec2 m_minPos = glm::vec2(0, 0);

	glm::vec2 m_maxPos = glm::vec2(0, 0);

};

class WEditorViewport : public WViewport
{
public:

	WEditorViewport(uint32 width, uint32 height, Format format);

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

	WSceneViewport(uint32 width, uint32 height, Format format);

	virtual ~WSceneViewport();

	virtual void ProcessInput() override;

private:

	

};