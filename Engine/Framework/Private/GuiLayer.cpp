#include "pch.h"
#include "Framework/Public/GuiLayer.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "RHI/Public/RHIHeads.h"

namespace WEngine
{
	GuiLayer::GuiLayer(std::string_view name)
		: Layer(name)
	{
		GuiConfigure guiConfigure = {};
		{
			guiConfigure.pInstance = RHIContext::GetInstance();
			guiConfigure.pGPU = RHIContext::GetGPU();
			guiConfigure.pDevice = RHIContext::GetDevice();
			guiConfigure.pQueue = RHIContext::GetQueue();
			guiConfigure.imageCount = RHIContext::g_maxFrames;
		}

		Gui* pGui = Gui::CreateGui(WEngine::Backend::Vulkan);
		pGui->Init(&guiConfigure);
		m_pPipeline = RHIContext::CreateRenderPipeline<GuiRenderPipeline>();
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		m_pPipeline->Init();
		m_pPipeline->Setup();
	}

	void GuiLayer::OnDettach()
	{
	}

	void GuiLayer::OnEvent(WEngine::Event* pEvent)
	{
	}

	void GuiLayer::OnUpdate(WEngine::TimeStep timeStep)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::DockSpaceOverViewport();
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			ImGui::Begin("Inspector");
			ImGui::End();

			ImGui::Begin("Display");
			ImGui::End();

		}
		ImGui::EndFrame();

		m_pPipeline->Execute();


		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}

	}
}