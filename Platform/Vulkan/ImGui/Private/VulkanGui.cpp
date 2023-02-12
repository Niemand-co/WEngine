#include "pch.h"
#include "Platform/Vulkan/ImGui/Public/VulkanGui.h"
#include "Platform/Vulkan/Public/VulkanHeads.h"
#include "Utils/Public/Window.h"
#include "Scene/Public/GameObject.h"

namespace Vulkan
{

	static void check_vk_result(VkResult err)
	{
		RE_ASSERT(err == VK_SUCCESS, "ImGui Failed.");
	}

	VulkanGui::VulkanGui()
	{
		ImGui::CreateContext();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::GetIO().ConfigFlags |= ImGuiCol_DockingEmptyBg;
		ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Window::cur_window->GetHandle()), true);
	}



	VulkanGui::~VulkanGui()
	{

	}

	void VulkanGui::Init(GuiConfigure* pConfigure)
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		VkDescriptorPool *pool = (VkDescriptorPool*)WEngine::Allocator::Get()->Allocate(sizeof(VkDescriptorPool));
		vkCreateDescriptorPool(static_cast<VulkanDevice*>(pConfigure->pDevice)->GetHandle(), &pool_info, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pool);

		{
			m_vulkanInitInfo.Instance = *static_cast<VulkanInstance*>(pConfigure->pInstance)->GetHandle();
			m_vulkanInitInfo.PhysicalDevice = *static_cast<VulkanGPU*>(pConfigure->pGPU)->GetHandle();
			m_vulkanInitInfo.Device = static_cast<VulkanDevice*>(pConfigure->pDevice)->GetHandle();
			m_vulkanInitInfo.Queue = static_cast<VulkanQueue*>(pConfigure->pQueue)->GetHandle();
			m_vulkanInitInfo.QueueFamily = static_cast<VulkanQueue*>(pConfigure->pQueue)->GetIndex();
			m_vulkanInitInfo.PipelineCache = VK_NULL_HANDLE;
			m_vulkanInitInfo.DescriptorPool = *pool;
			m_vulkanInitInfo.Subpass = 0;
			m_vulkanInitInfo.MinImageCount = 2;
			m_vulkanInitInfo.ImageCount = pConfigure->imageCount;
			m_vulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			m_vulkanInitInfo.Allocator = static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks();
			m_vulkanInitInfo.CheckVkResultFn = check_vk_result;
		}

		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_Tab] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_TabActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	}

	void VulkanGui::BindRenderPass(RHIRenderPass* pRenderPass, unsigned int subpass)
	{
		m_vulkanInitInfo.Subpass = subpass;
		ImGui_ImplVulkan_Init(&m_vulkanInitInfo, *static_cast<VulkanRenderPass*>(pRenderPass)->GetHandle());
	}

	ImTextureID VulkanGui::LoadTexture(RHITextureView* pTextureView, RHISampler* pSampler)
	{
		return (ImTextureID)ImGui_ImplVulkan_AddTexture(*static_cast<VulkanSampler*>(pSampler)->GetHandle(), *static_cast<VulkanTextureView*>(pTextureView)->GetHandle(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanGui::RemoveTexture(ImTextureID& ID)
	{
		ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet)ID);
	}

	void VulkanGui::RenderGUI(RHICommandBuffer *pCommandBuffer)
	{
		ImGui::Render();
		ImDrawData* data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(data, static_cast<VulkanCommandBuffer*>(pCommandBuffer)->GetHandle(), nullptr);
	}

	void VulkanGui::ShowInspector()
	{
		//WEngine::SRefl::TypeInfo<Material>::fields.EachVar([&](auto e)
		//{
		//	if constexpr (e.alist.count > 0)
		//	{
		//		constexpr auto property = e.alist.Get<0>();
		//		if constexpr (property == Range)
		//		{
		//			Gui::DrawSlider(e.name, mat->*(e.value), e.alist.Get<1>(), e.alist.Get<2>());
		//		}
		//		else if (property == Color)
		//		{
		//			//Gui::DrawColorEdit(e.name, (float*)&(mat->*(e.value)), true);
		//		}
		//	}
		//});
	}

	void VulkanGui::CollectComponents()
	{
		
	}

	void VulkanGui::CollectVariables()
	{
	}

}