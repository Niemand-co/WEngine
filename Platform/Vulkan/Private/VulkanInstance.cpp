#include "pch.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Platform/Vulkan/Public/VulkanGPU.h"
#include "Platform/Vulkan/Public/VulkanSurface.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace Vulkan
{

	WEngine::WArray<const char*> VulkanInstance::g_validationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_KHRONOS_synchronization2" };

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		const VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			RE_ERROR(pCallbackData->pMessage)
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			RE_WARN(pCallbackData->pMessage)
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
				RE_DEBUG(pCallbackData->pMessageIdName)
			else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
				RE_DEBUG(pCallbackData->pMessage)
		}
		return VK_FALSE;
	}

	VulkanInstance::VulkanInstance(bool enableDebugLayer, bool enableGPUValidation)
	{
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pApplicationName = "WEngine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "No_Engine";
		applicationInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		if (enableDebugLayer)
		{
			if (CheckLayerAvailability(g_validationLayers))
			{
				instanceCreateInfo.enabledLayerCount = g_validationLayers.Size();
				instanceCreateInfo.ppEnabledLayerNames = g_validationLayers.GetData();
			}
		}

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensionName = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		WEngine::WArray<const char*> extensions(glfwExtensionName, glfwExtensionName + glfwExtensionCount);
		if (enableDebugLayer)
		{
			extensions.Push(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		instanceCreateInfo.ppEnabledExtensionNames = extensions.GetData();
		instanceCreateInfo.enabledExtensionCount = extensions.Size();
		instanceCreateInfo.enabledLayerCount = 0;

		m_pInstance = (VkInstance*)WEngine::Allocator::Get()->Allocate(sizeof(VkInstance));
		Vulkan::VulkanAllocatorData userData = { WEngine::Allocator::Get()->Get() };
		VkAllocationCallbacks* callbacks = static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(&userData);
		RE_ASSERT(vkCreateInstance(&instanceCreateInfo, callbacks, m_pInstance) == VK_SUCCESS, "Failed to Create Vulkan Instance.");

		if (enableDebugLayer)
		{
			SetupDebugCallback();
		}

		CreateWin32Surface();
	}

	VulkanInstance::~VulkanInstance()
	{
		DestroyDebugUtilsMessengerEXT(*m_pInstance, m_debugUtilsMessenger, nullptr);
		Vulkan::VulkanAllocatorData userData = { WEngine::Allocator::Get()->Get() };
		VkAllocationCallbacks *callbacks = static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(&userData);
		vkDestroyInstance(*m_pInstance, callbacks);
	}

	void VulkanInstance::InitializeGPU()
	{
		unsigned int physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(*m_pInstance, &physicalDeviceCount, nullptr);
		VkPhysicalDevice *pPhysicalDevices = (VkPhysicalDevice*)WEngine::Allocator::Get()->Allocate(physicalDeviceCount * sizeof(VkPhysicalDevice));
		vkEnumeratePhysicalDevices(*m_pInstance, &physicalDeviceCount, pPhysicalDevices);

		m_gpus.Resize(physicalDeviceCount);

		for (unsigned int i = 0; i < physicalDeviceCount; ++i)
		{
			m_gpus[i] = new VulkanGPU(pPhysicalDevices + i);
		}
	}

	void VulkanInstance::UpdateSurface()
	{
		vkDestroySurfaceKHR(*m_pInstance, *m_pSurface, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks());

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(0);
		surfaceCreateInfo.hwnd = glfwGetWin32Window((GLFWwindow*)Window::Get()->GetHandle());

		auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(*m_pInstance, "vkCreateWin32SurfaceKHR");

		RE_ASSERT(CreateWin32SurfaceKHR(*m_pInstance, &surfaceCreateInfo, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), m_pSurface) == VK_SUCCESS, "Failed to Recreate Win32 Surface.");
	}

	VkInstance* VulkanInstance::GetHandle()
	{
		return m_pInstance;
	}

	void VulkanInstance::SetupDebugCallback()
	{
		VkDebugUtilsMessengerCreateInfoEXT debugMessageInfo = {};
		debugMessageInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
		debugMessageInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessageInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessageInfo.pfnUserCallback = debugCallback;
		debugMessageInfo.pUserData = nullptr;

		RE_ASSERT(CreateDebugUtilsMessengerEXT(*m_pInstance, &debugMessageInfo, nullptr, &m_debugUtilsMessenger) == VK_SUCCESS, "Failed to Create Debug Utils Messenger!")
	}

	void VulkanInstance::CreateWin32Surface()
	{
		
	}

	bool VulkanInstance::CheckLayerAvailability(const WEngine::WArray<const char*>& layers)
	{
		unsigned int layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		WEngine::WArray<VkLayerProperties> validateLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, validateLayers.GetData());

		for (const char* layerName : layers)
		{
			bool exist = false;
			for (const VkLayerProperties& layer : validateLayers)
			{
				if (strcmp(layerName, layer.layerName) == 0)
				{
					exist = true;
					break;
				}
			}
			if (!exist)
				return false;
		}
		return true;
	}

	VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* callback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_pInstance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, createInfo, pAllocator, callback);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_pInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

}