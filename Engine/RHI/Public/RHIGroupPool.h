#pragma once

class RHIGroup;
class RHIGroupLayout;

class RHIGroupPool
{
public:

	virtual ~RHIGroupPool() = default;

	virtual RHIGroup* GetGroup(unsigned int count = 1) = 0;

protected:

	RHIGroupLayout* m_pGroupLayout;

};