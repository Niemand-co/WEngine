#include "pch.h"
#include "Platform/Vulkan/ImGui/Public/VulkanGui.h"
#include "Utils/Public/Window.h"

namespace Vulkan
{

	VulkanGui::VulkanGui()
	{
		ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Window::cur_window->GetHandle()), true);
	}



	VulkanGui::~VulkanGui()
	{

	}

	void VulkanGui::Init(GuiConfigure* pConfigure)
	{
		ImGui_ImplVulkan_InitInfo vulkanInitInfo = {};
		{
			vulkanInitInfo.Allocator = static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks();
		}
	}

}