#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;
class RHIContext;

class RHITexture : public RHIResource
{
public:

	virtual ~RHITexture() = default;

	virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor* descriptor) = 0;

	virtual void LoadData(const WEngine::WString& path, RHIContext *context) = 0;

};