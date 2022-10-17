#pragma once

class RHITextureViewDescriptor;
class RHITextureView;
class RHIDevice;

class RHITexture
{
public:

	virtual ~RHITexture() = default;

	virtual RHITextureView* CreateTextureView(RHITextureViewDescriptor* descriptor) = 0;

public:

	static RHITexture LoadTextureFromDisk(std::string path);

};