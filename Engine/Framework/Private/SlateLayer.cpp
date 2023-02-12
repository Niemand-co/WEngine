#include "pch.h"
#include "Framework/Public/SlateLayer.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Editor/Public/Editor.h"
#include "Editor/Public/Screen.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Event/Public/Event.h"
#include "Event/Public/WindowEvent.h"
#include "Event/Public/MouseButtonEvent.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Platform/GLFWWindow/Public/GLFWWindow.h"
#include "Utils/ImGui/Panels/Public/SceneHierarchyPanel.h"
#include "Utils/ImGui/Panels/Public/InspectorPanel.h"
#include "Editor/Public/Ray.h"
#include "Render/Public/Viewport.h"

namespace WEngine
{
	SlateLayer::SlateLayer(const WString& name)
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

		Viewport = new WEditorViewport(1920, 1080, Format::A16R16G16B16_SFloat);

		m_imageID.Resize(RHIContext::g_maxFrames);
		//for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
			//m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
	}

	SlateLayer::~SlateLayer()
	{
	}

	void SlateLayer::OnAttach()
	{
		m_pHierarchyPanel = new SceneHierarchyPanel();

		m_pInspectorPanel = new InspectorPanel();
		
	}

	void SlateLayer::OnDettach()
	{
	}

	void SlateLayer::OnEvent(WEngine::Event* pEvent)
	{
		WEngine::EventDispatcher dispatcher(pEvent);

		dispatcher.Dispatch<WEngine::MouseButtonPressedEvent>([this](WEngine::MouseButtonPressedEvent* pEvent) -> bool
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			if (mousePos.x > m_displayArea.First().x && mousePos.x < m_displayArea.Second().x && mousePos.y >(m_displayArea.First().y) && mousePos.y < (m_displayArea.Second().y))
			{
				if(pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
					return false;
				else if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_1)
				{
					Editor::ClearSelection();

					CameraInfo* data = Editor::g_pEditorCamera->GetCameraInfo();
					ImVec2 mousePos = ImGui::GetMousePos();
					glm::vec2 pos = { (mousePos.x - m_displayArea.First().x) / (m_displayArea.Second().x - m_displayArea.First().x), (m_displayArea.Second().y - mousePos.y) / (m_displayArea.Second().y - m_displayArea.First().y) };
					Ray ray = Ray::GetClickRay(pos, data->Position, glm::inverse(data->MatrixV), glm::inverse(data->MatrixP));
					const WArray<GameObject*>& pGameObjects = GWorld::GetWorld()->GetGameObjects();
					for (GameObject* pGameObject : pGameObjects)
					{
						if (ray.IsIntersectWithGameObject(pGameObject))
						{
							Editor::SelectObject(pGameObject);
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
				Screen::SetWidth(m_displayArea.Second().x - m_displayArea.First().x);
				Screen::SetHeight(m_displayArea.Second().y - m_displayArea.First().y);
				if (Screen::SizeChanged())
				{
					for (unsigned int i = 0; i < RHIContext::g_maxFrames; ++i)
					{
						//Gui::g_pGui->RemoveTexture(m_imageID[i]);
						//m_imageID[i] = Gui::g_pGui->LoadTexture(Editor::g_pEditorCamera->GetRenderTarget(i).pColorTexture, m_pSampler);
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

	void SlateLayer::Tick(WEngine::TimeStep timeStep)
	{
		ENQUEUE_RENDERER_COMMAND([this]()
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

					if (ImGui::Begin("Display"))
					{
						ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
						ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
						ImVec2 viewportOffset = ImGui::GetWindowPos();
						//m_displayArea.First() = { viewportMin.x + viewportOffset.x, viewportMin.y + viewportOffset.y };
						//m_displayArea.Second() = { viewportMax.x + viewportOffset.x, viewportMax.y + viewportOffset.y };
						//m_displayArea.second.y -= 50;
						Screen::SetWidth(m_displayArea.Second().x - m_displayArea.Second().x);
						Screen::SetHeight(m_displayArea.Second().y - m_displayArea.Second().y);

						//ImGui::ImageButton(m_imageID[0], ImVec2(25, 25));
						ImGui::Image(m_imageID[RHIContext::g_currentFrame], { (float)Screen::GetWidth(), (float)Screen::GetHeight() });
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
		});

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}

		ENQUEUE_RENDERER_COMMAND([]()
		{
			ImDrawData *Data = ImGui::GetDrawData();
			{

			}
		});

	}
}