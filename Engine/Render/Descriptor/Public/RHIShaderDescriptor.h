#pragma once

class RHIShaderDescriptor
{
public:

	RHIShaderDescriptor();

	virtual ~RHIShaderDescriptor();

public:

	unsigned int shaderStage;

	unsigned int* pCode;

	size_t codeSize;

	std::string entryName;

};