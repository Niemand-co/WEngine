#pragma once
#include "Render/Descriptor/Public/RHIDescriptor.h"

class RHIShaderDescriptor : public RHIDescriptor
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