#pragma once

class RHIVertexInputState : public RHIResource
{
public:

	RHIVertexInputState(const WEngine::WArray<class VertexInputElement>& InElements)
		: Elements(InElements)
	{
	}

	virtual ~RHIVertexInputState() = default;

protected:

	WEngine::WArray<struct VertexInputElement> Elements;

};