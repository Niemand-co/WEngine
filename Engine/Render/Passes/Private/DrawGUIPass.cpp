#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Utils/Public/Window.h"

DrawGUIPass::DrawGUIPass(RenderPassConfigure *pConfigure)
	: ScriptableRenderPass(pConfigure)
{
	ImGui::CreateContext();
	m_io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Window::cur_window->GetHandle()), true);

}

DrawGUIPass::~DrawGUIPass()
{
}

void DrawGUIPass::Setup(RHIContext* context, CameraData* cameraData)
{
}

void DrawGUIPass::Execute(RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{
}
