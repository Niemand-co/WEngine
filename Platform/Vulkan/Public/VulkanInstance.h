#pragma once
#ifndef WENGINE_VULKAN_RHI_H
#define WENGINE_VULKAN_RHI_H

#include "RHI/Public/RHIInstance.h"

namespace Vulkan
{

	class VulkanInstance : public RHIInstance
	{
	public:

		VulkanInstance(bool enableDebugLayer = false, bool enableGPUValidation = false);

		virtual ~VulkanInstance();

		virtual void InitializeGPU() override;

		VkInstance GetHandle() const { return Instance; }

	private:

		void SetupDebugCallback();

	private:

		bool CheckLayerAvailability(const WEngine::WArray<const char*>& layers);

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* callback);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

	public:

		static WEngine::WArray<const char*> g_validationLayers;

	private:

		VkInstance Instance;

		VkDebugUtilsMessengerEXT m_debugUtilsMessenger;
	};

}


#endif