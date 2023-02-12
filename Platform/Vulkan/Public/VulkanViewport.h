#pragma once
#include "RHI/Public/RHIViewport.h"

namespace Vulkan
{

	class VulkanViewport : public RHIViewport
	{
	public:

		VulkanViewport(class VulkanDevice* pInDevice);

		virtual ~VulkanViewport();

		virtual void Tick() override;

		virtual uint32 AcquireImageIndex() override;

		virtual void Present() override;

	private:

		VulkanDevice *pDevice;

	};

}