#pragma once

class RHIGroup;
class RHIGroupLayout;

class RHIGroupPool : public RHIResource
{
public:

	virtual ~RHIGroupPool() = default;

protected:

	RHIGroupLayout* m_pGroupLayout;

};