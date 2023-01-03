#pragma once

class RHIScissorDescriptor
{
public:

	RHIScissorDescriptor() = default;

	~RHIScissorDescriptor() = default;

public:

	int offsetX = 0;

	int offsetY = 0;

	unsigned int width = 0;

	unsigned int height = 0;

};