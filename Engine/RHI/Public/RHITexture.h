#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;
class RHIContext;

class RHITexture
{
public:

	virtual ~RHITexture() = default;

	virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor* descriptor) = 0;

	virtual void LoadData(std::string path, RHIContext *context) = 0;

};