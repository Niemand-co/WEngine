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

	void GuiLayer::OnDettach()
	{
	}

	void GuiLayer::OnEvent(WEngine::Event* pEvent)
	{
		WEngine::EventDispatcher dispatcher(pEvent);

		dispatcher.Dispatch<WEngine::MouseButtonPressedEvent>([this](WEngine::MouseButtonPressedEvent* pEvent) -> bool
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			if (mousePos.x > m_displayArea.first.x && mousePos.x < m_displayArea.second.x && mousePos.y >(m_displayArea.first.y + 50) && mousePos.y < (m_displayArea.second.y + 50))
			{
				if(pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_2)
					return false;
				else if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_1)
				{
					Editor::ClearSelection();

					CameraData* data = Editor::g_pEditorCamera->GetData();
					ImVec2 mousePos = ImGui::GetMousePos();
					glm::vec2 pos = { (mousePos.x - m_displayArea.first.x) / (m_displayArea.second.x - m_displayArea.first.x), (m_displayArea.second.y - mousePos.y) / (m_displayArea.second.y - m_displayArea.first.y) };
					glm::vec4 ray_clip = glm::vec4(pos.x * 2.0f - 1.0f, pos.y * 2.0f - 1.0f, -1.0f, 1.0f);
					ray_clip = glm::inverse(data->MatrixVP) * ray_clip;
					glm::vec3 dir = ray_clip / ray_clip.w;
					//Ray ray = Ray::GetClickRay(pos, data->Position, glm::inverse(data->MatrixVP));
					Ray ray(data->Position, glm::normalize(dir - data->Position));
					const std::vector<GameObject*>& pGameObjects = World::GetWorld()->GetGameObjects();
					for (GameObject* pGameObject : pGameObjects)
					{
						if (ray.IsIntersectWithGameObject(pGameObject))
						{
							Editor::SelectObject(pGameObject);
							return true;
						}
					}
					if(ray.IsIntersectWithTriangle(glm::vec3(-10.0f, -10.0f, 1.0f), glm::vec3(10.0f, -10.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f)))
						RE_LOG("Triangle");
				}
			}
			return true;
		});

		dispatcher.Dispatch<WEngine::MouseButtonReleasedEvent>([this](WEngine::MouseButtonReleasedEvent* pEvent) -> bool
		{
			if (pEvent->GetMouseCode() == GLFW_MOUSE_BUTTON_1)
			{
				Screen::SetWidth(m_displayArea.second.x - m_displayArea.first.x);
				Screen::SetHeight(m_displayArea.second.y - m_displayArea.first.y);
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
				ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
				ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
				ImVec2 viewportOffset = ImGui::GetWindowPos();
				m_displayArea.first = { viewportMin.x + viewportOffset.x, viewportMin.y + viewportOffset.y };
				m_displayArea.second = { viewportMax.x + viewportOffset.x, viewportMax.y + viewportOffset.y };
				m_displayArea.second.y -= 50;

				Screen::SetWidth(m_displayArea.second.x - m_displayArea.first.x);
				Screen::SetHeight(m_displayArea.second.y - m_displayArea.first.y);

				ImGui::ImageButton(m_imageID[0], ImVec2(25, 25));
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