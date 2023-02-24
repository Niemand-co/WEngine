#pragma once
#include "RHI/Public/RHIFence.h"

namespace Vulkan
{

	class VulkanFence : public RHIFence
	{
	public:

		VulkanFence(class VulkanDevice *pInDevice);

		virtual ~VulkanFence();

		virtual void Reset() override;

		virtual bool Wait(double Time = UINT64_MAX) override;

		bool IsFenceSignaled() const { return State == EFenceState::Signaled; }

		VkFence GetHandle() const { return Fence; }

	private:

		enum EFenceState : uint8
		{
			NotReady,
			Signaled,
		};

		EFenceState State;

		VkFence Fence;

		VulkanDevice *pDevice;

	};

}