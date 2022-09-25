#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

enum class RHIBackend
{
	None = 0,
	Vulkan,
	DirectX12,
	OpenGL
};

class RHIInstanceDescriptor : public RHIDescriptor
{
public:

	RHIInstanceDescriptor();

	virtual ~RHIInstanceDescriptor();

public:
	RHIBackend backend;
	bool enableDebugLayer;
	bool enableGPUValidator;
};