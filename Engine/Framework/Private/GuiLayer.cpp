#include "pch.h"
#include "Framework/Public/GuiLayer.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Editor/Public/Editor.h"
#include "Editor/Public/Screen.h"
#include "Scene/Components/Public/Camera.h"

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

		RHITextureDescriptor textureDescriptor = {};
		{
			textureDescriptor.format = Format::A8R8G8B8_UNorm;
			textureDescriptor.width = 477;
			textureDescriptor.height = 377;
			textureDescriptor.layout = AttachmentLayout::Undefined;
			textureDescriptor.mipCount = 1;
			textureDescriptor.usage = IMAGE_USAGE_TRANSFER_DST | IMAGE_USAGE_SAMPLED;
		}
		m_pTexture = RHIContext::GetDevice()->CreateTexture(&textureDescriptor);
		m_pTexture->LoadData("assets/chino.png", RHIContext::GetContext());

		RHISamplerDescriptor samplerDescriptor = {};
		{
			samplerDescriptor.minFilter = Filter::Linear;
			samplerDescriptor.magFilter = Filter::Linear;
		}
		m_pSampler = RHIContext::GetDevice()->CreateSampler(&samplerDescriptor);

		RHITextureViewDescriptor uvd = {};
		{
			uvd.format = Format::A8R8G8B8_UNorm;
			uvd.arrayLayerCount = 1;
			uvd.baseArrayLayer = 0;
			uvd.mipCount = 1;
			uvd.baseMipLevel = 0;
			uvd.dimension = Dimension::Texture2D;
			uvd.imageAspect = IMAGE_ASPECT_COLOR;
		}

		
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
		m_imageID = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(RHIContext::g_currentFrame).pColorTexture, m_pSampler);

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
			ImGui::Image(m_imageID, ImVec2(Screen::GetWidth(), Screen::GetHeight()));
			ImGui::End();

			ImGui::Begin("Console");
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