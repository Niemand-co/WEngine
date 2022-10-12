#pragma once
#include "RHI/Public/RHIEvent.h"

class VulkanEvent : public RHIEvent
{
public:

	VulkanEvent(VkEvent *pEvent);

	virtual ~VulkanEvent();

	VkEvent* GetHandle();

private:

	VkEvent *m_pEvnet;

};