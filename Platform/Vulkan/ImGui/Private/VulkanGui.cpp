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
		m_io = ImGui::GetIO();
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
		vkCreateDescriptorPool(*static_cast<VulkanDevice*>(pConfigure->pDevice)->GetHandle(), &pool_info, static_cast<VulkanAllocator*>(WEngine::Allocator::Get())->GetCallbacks(), pool);

		{
			m_vulkanInitInfo.Instance = *static_cast<VulkanInstance*>(pConfigure->pInstance)->GetHandle();
			m_vulkanInitInfo.PhysicalDevice = *static_cast<VulkanGPU*>(pConfigure->pGPU)->GetHandle();
			m_vulkanInitInfo.Device = *static_cast<VulkanDevice*>(pConfigure->pDevice)->GetHandle();
			m_vulkanInitInfo.Queue = *static_cast<VulkanQueue*>(pConfigure->pQueue)->GetHandle();
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
	}

	void VulkanGui::BindRenderPass(RHIRenderPass* pRenderPass, unsigned int subpass)
	{
		m_vulkanInitInfo.Subpass = subpass;
		ImGui_ImplVulkan_Init(&m_vulkanInitInfo, *static_cast<VulkanRenderPass*>(pRenderPass)->GetHandle());
	}

	void VulkanGui::RenderGUI(RHICommandBuffer *pCommandBuffer)
	{
		ImGui::Render();
		ImDrawData* data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(data, *static_cast<VulkanCommandBuffer*>(pCommandBuffer)->GetHandle(), nullptr);
	}

	void VulkanGui::ShowInspector()
	{
		if(m_pGameObject == nullptr)
			return;
		Material *mat = m_pGameObject->GetComponent<Material>();
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