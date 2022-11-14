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
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Utils/ImGui/Panels/Public/SceneHierarchyPanel.h"
#include "Utils/ImGui/Panels/Public/InspectorPanel.h"
#include "Editor/Public/Ray.h"

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
		for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
			m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		m_pPipeline->Init();
		m_pPipeline->Setup();

		m_pHierarchyPanel = new SceneHierarchyPanel();

		m_pInspectorPanel = new InspectorPanel();
	}

	void GuiLayer::OnDettach()
	{
	}

	void GuiLayer::OnEvent(WEngine::Event* pEvent)
	{
		WEngine::EventDispatcher dispatcher(pEvent);

		dispatcher.Dispatch<WEngine::MouseButtonPressedEvent>([this](WEngine::MouseButtonPressedEvent* pEvent) -> bool
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			if (mousePos.x > m_displayArea.first.x && mousePos.x < (m_displayArea.first.x + m_displayArea.second.x) && mousePos.y >(m_displayArea.first.y + 50) && mousePos.y < (m_displayArea.first.y + m_displayArea.second.y + 50))
			{
				if(pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
					return false;
				else if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_1)
				{
					Editor::ClearSelection();

					CameraData* data = Editor::g_pEditorCamera->GetData();
					ImVec2 mousePos = ImGui::GetMousePos();
					glm::vec4 ScreenPos = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
					ScreenPos.x = ((mousePos.x - m_displayArea.first.x) / m_displayArea.second.x) * 2.0f - 1.0f;
					ScreenPos.y = ((mousePos.y - m_displayArea.first.y) / m_displayArea.second.y) * 2.0f - 1.0f;
					ScreenPos = glm::inverse(data->MatrixVP) * ScreenPos;
					ScreenPos /= ScreenPos.w;
					Ray ray(data->Position, glm::normalize(glm::vec3(ScreenPos) - data->Position));
					const std::vector<GameObject*>& pGameObjects = World::GetWorld()->GetGameObjects();
					for (GameObject* pGameObject : pGameObjects)
					{
						if (ray.IsIntersectWithGameObject(pGameObject))
						{
							Editor::SelectObject(pGameObject);
							RE_LOG(pGameObject->GetName());
							return true;
						}
					}
				}
			}
			return true;
		});

		dispatcher.Dispatch<WEngine::MouseButtonReleasedEvent>([this](WEngine::MouseButtonReleasedEvent* pEvent) -> bool
		{
			if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_1)
			{
				Screen::SetWidth(m_displayArea.second.x);
				Screen::SetHeight(m_displayArea.second.y);
				if (Screen::SizeChanged())
				{
					Screen::g_displayingCamera->RecreateRenderTarget(Screen::GetWidth(), Screen::GetHeight());
					for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
					{
						Gui::g_pGui->RemoveTexture(m_imageID[i]);
						m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
					}
					Screen::ResetState();
				}
			}
			return false;
		});

		dispatcher.Dispatch<WEngine::WindowResizeEvent>([this](WEngine::WindowResizeEvent* pEvent) -> bool
		{
			//Screen::SetWidth(m_displayArea.second.x);
			//Screen::SetHeight(m_displayArea.second.y);
			//if (Screen::SizeChanged())
			//{
			//	Screen::g_displayingCamera->RecreateRenderTarget(Screen::GetWidth(), Screen::GetHeight());
			//	for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
			//	{
			//		Gui::g_pGui->RemoveTexture(m_imageID[i]);
			//		m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
			//	}
			//	Screen::ResetState();
			//}
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
				if (ImGui::BeginMenu("View"))
				{
					ImGui::MenuItem("Hierarchy", "", &m_isHierarchyShowed);
					ImGui::MenuItem("Console", "", &m_isConsoleShowed);
					ImGui::MenuItem("Settings", "", &m_isSettingsShowed);
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			if (ImGui::Begin("Inspector"))
			{
				m_pInspectorPanel->DrawGameObject();
			}
			ImGui::End();

			if(ImGui::Begin("Display"))
			{
				m_displayArea.first = ImGui::GetItemRectMin();
				m_displayArea.second = ImGui::GetContentRegionAvail();
				m_displayArea.second.y -= 50;

				Screen::SetWidth(m_displayArea.second.x);
				Screen::SetHeight(m_displayArea.second.y);

				ImGui::ImageButton(m_imageID[0], ImVec2(25, 25));
				ImGui::Image(m_imageID[RHIContext::g_currentFrame], m_displayArea.second);
			}
			ImGui::End();

			if (m_isHierarchyShowed)
			{
				if (ImGui::Begin("Hierarchy", &m_isHierarchyShowed))
				{
					m_pHierarchyPanel->DrawNodes();
				}
				ImGui::End();
			}

			if (m_isConsoleShowed)
			{
				if (ImGui::Begin("Console", &m_isConsoleShowed))
				{
				}
				ImGui::End();
			}

			if (m_isSettingsShowed)
			{
				if (ImGui::Begin("Settings", &m_isSettingsShowed))
				{

				}
				ImGui::End();
			}

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