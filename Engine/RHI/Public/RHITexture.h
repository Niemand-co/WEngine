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

	virtual void LoadData(const WEngine::WString& path, RHIContext *context) = 0;

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

};