#pragma once
#include "Utils/ImGui/Public/Gui.h"

namespace Vulkan
{

	class VulkanGui : public Gui
	{
	public:

		VulkanGui();

		virtual ~VulkanGui();

		virtual void Init(GuiConfigure* pConfigure) override;

		virtual void BindRenderPass(RHIRenderPass *pRenderPass, unsigned int subpass) override;
		
		virtual ImTextureID LoadTexture(RHITextureView *pTextureView, RHISampler *pSampler) override;

		virtual void RenderGUI(RHICommandBuffer* pCommandBuffer) override;

		virtual void ShowInspector() override;

		virtual void CollectComponents() override;

		virtual void CollectVariables() override;

	private:

		VkInstance *m_pInstance;

		VkPhysicalDevice *m_pPhysicalDevice;

		VkDevice *m_pDevice;

		VkDescriptorPool *m_pDescriptorPool;

		VkCommandPool *m_pCommandPool;

		ImGui_ImplVulkan_InitInfo m_vulkanInitInfo;

	};

}