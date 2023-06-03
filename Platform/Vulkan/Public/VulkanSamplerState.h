#pragma once

namespace Vulkan
{

	class VulkanSamplerState : public RHISamplerState
	{
	public:

		VulkanSamplerState(class VulkanDevice *pInDevice, const VkSamplerCreateInfo& Info);

		virtual ~VulkanSamplerState() = default;

		static void SetupCreateInfo(const RHISamplerStateInitializer& Initializer, VulkanDevice* pDevice, VkSamplerCreateInfo& OutInfo);

		VkSampler GetHandle() const { return Sampler; }

	private:

		VkSampler Sampler;

	};

	class VulkanSamplerStateManager : public VulkanResource
	{
	public:

		VulkanSamplerStateManager(class VulkanDevice* pInDevice)
			: pDevice(pInDevice)
		{
		}

		VulkanSamplerState* GetSamplerState(uint32 Key)
		{
			VulkanSamplerState *SamplerState = nullptr;
			{
				WEngine::WScopeLock Lock(&SamplerStateLock);
				if (SamplerStates.Find(Key))
				{
					return SamplerStates[Key];
				}
			}
			return nullptr;
		}

		void AddSamplerState(uint32 Key, VulkanSamplerState* SamplerState)
		{
			WEngine::WScopeLock Lock(&SamplerStateLock);
			SamplerStates.Insert(Key, SamplerState);
		}

	private:

		VulkanDevice *pDevice;

		WEngine::WHashMap<uint32, VulkanSamplerState*> SamplerStates;

		WCriticalSection SamplerStateLock;

		friend class VulkanDevice;

	};

}