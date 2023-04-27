#pragma once
#include "Render/Public/RenderResource.h"

class RHITexture;

class WTexture : public RenderResource
{
public:

	WTexture() = default;

	virtual ~WTexture() = default;

private:

	WEngine::WString Name;

};