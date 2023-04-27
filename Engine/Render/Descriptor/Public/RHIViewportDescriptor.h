#pragma once

class RHIViewportDescriptor
{
public:

	RHIViewportDescriptor() = default;

	~RHIViewportDescriptor() = default;

public:

    uint32 Width = 0;

    uint32 Height = 0;

	EFormat format = EFormat::A16R16G16B16_SFloat;

};