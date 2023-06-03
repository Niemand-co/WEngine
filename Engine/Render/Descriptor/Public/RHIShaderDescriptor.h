#pragma once

class RHIShaderDescriptor
{
public:

	RHIShaderDescriptor();

	~RHIShaderDescriptor() = default;

public:

	uint32* Code;

	uint32 CodeSize;

	WEngine::WString EntryName;

	EShaderFrequency Frequency;

};