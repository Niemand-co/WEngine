#pragma once

class RHIShaderDescriptor
{
public:

	RHIShaderDescriptor();

	virtual ~RHIShaderDescriptor();

public:

	ShaderStage shaderStage;

	unsigned int* pCode;

	size_t codeSize;

	std::string entryName;

};