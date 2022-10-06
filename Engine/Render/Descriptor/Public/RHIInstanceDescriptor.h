#pragma once

enum class RHIBackend
{
	None = 0,
	Vulkan,
	DirectX12,
	OpenGL
};

class RHIInstanceDescriptor
{
public:

	RHIInstanceDescriptor();

	virtual ~RHIInstanceDescriptor();

public:
	RHIBackend backend;
	bool enableDebugLayer;
	bool enableGPUValidator;
};