#pragma once

class RHIShaderDescriptor
{
public:

	RHIShaderDescriptor();

	~RHIShaderDescriptor() = default;

public:

	unsigned int shaderStage;

	unsigned int* pCode;

	size_t codeSize;

	std::string entryName;

};