#include "pch.h"
#include "Framework/Public/GuiLayer.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Editor/Public/Editor.h"
#include "Editor/Public/Screen.h"
#include "Scene/Components/Public/Camera.h"
#include "Event/Public/Event.h"
#include "Event/Public/WindowEvent.h"
#include "Event/Public/MouseButtonEvent.h"
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

		m_imageID.resize(RHIContext::g_maxFrames);
		for(unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
			m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
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
		WEngine::EventDispatcher dispatcher(pEvent);

		dispatcher.Dispatch<WEngine::MouseButtonPressedEvent>([this](WEngine::MouseButtonPressedEvent* pEvent) -> bool
		{
			if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				if(mousePos.x > m_displayArea.first.x && mousePos.x < m_displayArea.second.x && mousePos.y > m_displayArea.first.y && mousePos.y < m_displayArea.second.y)
					return false;
			}
			return true;
		});

		dispatcher.Dispatch<WEngine::MouseButtonReleasedEvent>([this](WEngine::MouseButtonReleasedEvent* pEvent) -> bool
		{
			return false;
		});

		dispatcher.Dispatch<WEngine::WindowResizeEvent>([this](WEngine::WindowResizeEvent* pEvent) -> bool
		{
			//m_displayArea.first = ImGui::GetItemRectMin();
			//m_displayArea.second = ImGui::GetItemRectMax();
			//Screen::SetWidth(m_displayArea.second.x - m_displayArea.first.x);
			//Screen::SetHeight(m_displayArea.second.y - m_displayArea.first.y);
			return false;
		});
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
			m_displayArea.first = ImGui::GetItemRectMin();
			m_displayArea.second = ImGui::GetWindowSize();
			Screen::SetWidth(m_displayArea.second.x);
			Screen::SetHeight(m_displayArea.second.y);
			if (Screen::SizeChanged())
			{
				Screen::g_displayingCamera->RecreateRenderTarget(Screen::GetWidth(), Screen::GetHeight());
				for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
					m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
			}
			ImGui::Image(m_imageID[RHIContext::g_currentFrame], ImVec2(Screen::GetWidth(), Screen::GetHeight()));
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