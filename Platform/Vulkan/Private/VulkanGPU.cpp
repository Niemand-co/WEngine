#include "pch.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/Vulkan/Public/VulkanMacro.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Platform/Vulkan/Public/VulkanQueue.h"
#include "Render/Descriptor/Public/RHIDeviceDescriptor.h"
#include "Render/Descriptor/Public/RHIQueueDescriptor.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanGPU::VulkanGPU(VkPhysicalDevice physicalDevice, VkSurfaceKHR *surface)
	{
		m_device = physicalDevice;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_device, &properties);

		GPUName = properties.deviceName;
		RE_LOG(GPUName);

		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_DISCRETE_GPU;
		else if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_INTEGRATED_GPU;
		else
			m_feature.PHYSICAL_DEVICE_TYPE |= DEVICE_TYPE_OTHER;

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(m_device, &features);

		if(features.geometryShader)
			m_feature.SHDAER_SUPPORT |= FEATURE_GEOMETRY_SHADER;
		if(features.tessellationShader)
			m_feature.SHDAER_SUPPORT |= FEATURE_TESSELATION_SHADER;

		unsigned int queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyPropertyCount, queueFamilyProperties.data());

		VkBool32 isPresentSupported;
		for (unsigned int i = 0; i < queueFamilyPropertyCount; ++i)
		{
			VkQueueFamilyProperties properties = queueFamilyProperties[i];
			vkGetPhysicalDeviceSurfaceSupportKHR(m_device, i, *surface, &isPresentSupported);
			QueueProperty* queueProperty = new QueueProperty();
			queueProperty->count = properties.queueCount;
			queueProperty->QUEUE_SUPPORT = properties.queueFlags;
			if (isPresentSupported)
				queueProperty->QUEUE_SUPPORT |= QUEUE_PROPERTY_PRESENT;
			m_feature.queueProperties.push_back(queueProperty);
		}
	}

	VulkanGPU::~VulkanGPU()
	{
	}

	const GPUFeature& VulkanGPU::GetFeature() const
	{
		return RHIGPU::m_feature;
	}

	const std::string& VulkanGPU::GetGPUName() const
	{
		return RHIGPU::GPUName;
	}

	RHIDevice* VulkanGPU::CreateDevice(RHIDeviceDescriptor *descriptor)
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(descriptor->queueInfoCount);
		std::vector<QueueStack> queueStack;
		queueStack.reserve(descriptor->queueInfoCount);
		unsigned int size = 0;
		for (unsigned int i = 0; i < descriptor->queueInfoCount; ++i)
		{
			RHIQueueDescriptor queueDescriptor = descriptor->queueInfos[i];
			unsigned int queueFamilyIndex = 0;
			unsigned int flag = 0u;
			switch (queueDescriptor.type)
			{
				case RHIQueueType::Graphics:
					flag |= QUEUE_PROPERTY_GRAPHICS;
					break;
				case RHIQueueType::Compute:
					flag |= QUEUE_PROPERTY_COMPUTE;
					break;
				case RHIQueueType::Blit:
					flag |= QUEUE_PROPERTY_BLIT;
					break;
				case RHIQueueType::Present:
					flag |= QUEUE_PROPERTY_PRESENT;
					break;
			}
			for (; queueFamilyIndex < m_feature.queueProperties.size(); ++queueFamilyIndex)
			{
				QueueProperty *queueProperty = m_feature.queueProperties[queueFamilyIndex];
				if ((queueProperty->QUEUE_SUPPORT & flag) > 0 && queueDescriptor.count <= queueProperty->count)
					break;
			}
			if(queueFamilyIndex >= m_feature.queueProperties.size())
				RE_ASSERT(false, "GPU does not support queue specified.")
			else
			{
				queueStack.push_back({queueDescriptor.count, queueFamilyIndex, queueDescriptor.type});
				size += queueDescriptor.count;
			}

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = queueDescriptor.count;
			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos[i] = queueCreateInfo;
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo deviceCreateInfo = {};
		std::vector<const char*> extensionNames = { "VK_KHR_swapchain" };
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.ppEnabledExtensionNames = extensionNames.data();
		deviceCreateInfo.enabledExtensionCount = extensionNames.size();
		if (descriptor->enableDebugLayer)
		{
			deviceCreateInfo.enabledLayerCount = static_cast<unsigned int>(VulkanInstance::g_validationLayers.size());
			deviceCreateInfo.ppEnabledLayerNames = VulkanInstance::g_validationLayers.data();
		}
		else
		{
			deviceCreateInfo.enabledLayerCount = 0u;
		}

		VkDevice *pDevice = (VkDevice*)Allocator::Allocate(sizeof(VkDevice));
		RE_ASSERT(vkCreateDevice(m_device, &deviceCreateInfo, nullptr, pDevice) == VK_SUCCESS, "Failed to Create Device.");

		RHIDevice* device = new VulkanDevice(pDevice, queueStack);

		return device;
	}

}