#pragma once
#include "Render/Mesh/Public/Vertex.h"

class RHIVertexInputState : public RHIResource
{
public:

	RHIVertexInputState(const WEngine::WArray<VertexInputElement>& InElements)
		: Elements(InElements)
	{
	}

	virtual ~RHIVertexInputState() = default;

protected:

	WEngine::WArray<VertexInputElement> Elements;

};