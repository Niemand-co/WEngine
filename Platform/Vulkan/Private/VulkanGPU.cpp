#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Render/Descriptor/Public/RHIDeviceDescriptor.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanGPU::VulkanGPU(VkPhysicalDevice * pPhysicalDevice)
	{
		m_pPhysicalDevice = pPhysicalDevice;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(*m_pPhysicalDevice, &properties);

		GPUName = properties.deviceName;
		RE_LOG(GPUName);

	}

	VulkanGPU::~VulkanGPU()
	{
	}

	const WEngine::WString& VulkanGPU::GetGPUName() const
	{
		return RHIGPU::GPUName;
	}

	RHIDevice* VulkanGPU::CreateDevice(RHIDeviceDescriptor *descriptor)
	{
		WEngine::WArray<VkDeviceQueueCreateInfo> QueueCreateInfos(descriptor->queueInfoCount);
		WEngine::WArray<QueueStack> queueStack;
		queueStack.Reserve(descriptor->queueInfoCount);
		unsigned int size = 0;

		unsigned int queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(*m_pPhysicalDevice, &queueFamilyPropertyCount, nullptr);
		WEngine::WArray<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(*m_pPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.GetData());

		for (unsigned int i = 0; i < descriptor->queueInfoCount; ++i)
		{
			RHIQueueDescriptor queueDescriptor = descriptor->queueInfos[i];
			unsigned int QueueFamilyIndex = 0;
			for (; QueueFamilyIndex < queueFamilyProperties.Size(); ++QueueFamilyIndex)
			{
				VkQueueFamilyProperties queueProperty = queueFamilyProperties[QueueFamilyIndex];
				if ((queueProperty.queueFlags & (uint8)queueDescriptor.type) > 0 && queueDescriptor.count <= queueProperty.queueCount)
					break;
			}
			if(QueueFamilyIndex >= queueFamilyProperties.Size())
				RE_ASSERT(false, "GPU does not support queue specified.")
			else
			{
				queueStack.Push({queueDescriptor.count, QueueFamilyIndex, queueDescriptor.type});
				size += queueDescriptor.count;
			}

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = QueueFamilyIndex;
			queueCreateInfo.queueCount = queueDescriptor.count;
			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			QueueCreateInfos[i] = queueCreateInfo;
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		{
			deviceFeatures.geometryShader = true;
		}

		VkDeviceCreateInfo DeviceCreateInfo = {};
		WEngine::WArray<const char*> extensionNames = { "VK_KHR_swapchain" };
		DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.GetData();
		DeviceCreateInfo.queueCreateInfoCount = QueueCreateInfos.Size();
		DeviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		DeviceCreateInfo.ppEnabledExtensionNames = extensionNames.GetData();
		DeviceCreateInfo.enabledExtensionCount = extensionNames.Size();
		if (descriptor->enableDebugLayer)
		{
			DeviceCreateInfo.enabledLayerCount = static_cast<unsigned int>(VulkanInstance::g_validationLayers.Size());
			DeviceCreateInfo.ppEnabledLayerNames = VulkanInstance::g_validationLayers.GetData();
		}
		else
		{
			DeviceCreateInfo.enabledLayerCount = 0u;
		}

		return new VulkanDevice(this, &DeviceCreateInfo, queueStack);
	}

}