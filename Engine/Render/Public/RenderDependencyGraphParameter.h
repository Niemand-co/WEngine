#pragma once
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"

class ParametersLayout
{

};

class WRDGParameterStruct
{
public:

	WRDGParameterStruct();

	~WRDGParameterStruct();

	void InitializeLayout();

	RHIRenderPassDescriptor GetRenderPassInfo() const;

	bool HasRenderTargets() const;

	bool HasExternalOutput() const;

private:

	const uint8* Contents;

	const ParametersLayout *Layout;

};