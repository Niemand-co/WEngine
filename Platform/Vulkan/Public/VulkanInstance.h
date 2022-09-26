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

	private:

		void SetupDebugCallback();

		void CreateWin32Surface();

	private:

		bool CheckLayerAvailability(const std::vector<const char*>& layers);

		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* callback);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);

	public:

		static std::vector<const char*> g_validationLayers;

	private:

		VkInstance m_instance;

		VkSurfaceKHR *m_surface;

		VkDebugUtilsMessengerEXT m_debugUtilsMessenger;
	};

}


#endif