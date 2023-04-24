#pragma once

class RHIGroup;
class RHIGroupLayout;

class RHIGroupPool : public RHIResource
{
public:

	RHIGroupPool()
		: RHIResource(ERHIResourceType::RRT_None)
	{
	}

	virtual ~RHIGroupPool() = default;

protected:

	RHIGroupLayout* m_pGroupLayout;

};