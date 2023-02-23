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

		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_DISCRETE_GPU;
		else if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_INTEGRATED_GPU;
		else
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_OTHER;

		m_feature.minUBOAlignment = properties.limits.minUniformBufferOffsetAlignment;

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(*m_pPhysicalDevice, &features);

		if(features.geometryShader)
			m_feature.SHDAER_SUPPORT |= FEATURE_GEOMETRY_SHADER;
		if(features.tessellationShader)
			m_feature.SHDAER_SUPPORT |= FEATURE_TESSELATION_SHADER;

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(*m_pPhysicalDevice, &memoryProperties);
		m_feature.memorySupports.Resize(memoryProperties.memoryHeapCount);
		for (unsigned int i = 0; i < memoryProperties.memoryHeapCount; ++i)
		{
			m_feature.memorySupports[i] = new MemoryTypeSupport();
			if(memoryProperties.memoryHeaps[i].flags == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				m_feature.memorySupports[i]->type = MemoryType::LocalMemory;
			else
				m_feature.memorySupports[i]->type = MemoryType::HostMemory;
			m_feature.memorySupports[i]->size = memoryProperties.memoryHeaps[i].size;
		}
		for (unsigned int i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			m_feature.memorySupports[memoryProperties.memoryTypes[i].heapIndex]->properties |= memoryProperties.memoryTypes[i].propertyFlags;
		}

	}

	VulkanGPU::~VulkanGPU()
	{
	}

	const GPUFeature& VulkanGPU::GetFeature() const
	{
		return RHIGPU::m_feature;
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

		return new VulkanDevice(this, &DeviceCreateInfo);
	}

}