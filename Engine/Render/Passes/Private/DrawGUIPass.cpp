#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Public/RHIHeads.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"

DrawGUIPass::DrawGUIPass(RenderPassConfigure *pConfigure)
	: ScriptableRenderPass(pConfigure)
{

}

DrawGUIPass::~DrawGUIPass()
{
}

void DrawGUIPass::Setup(RHIContext* context, CameraData* cameraData)
{
	RHIPipelineStateObjectDescriptor psoDescriptor = {};
	{
		psoDescriptor.renderPass = m_pRenderPass;
	}
	m_pPSO = context->CreatePSO(&psoDescriptor);

	m_pRenderTargets.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { context->GetTextureView(i) };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = Window::cur_window->GetWidth();
			renderTargetDescriptor.height = Window::cur_window->GetHeight();
		}
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
	
	Gui::g_pGui->BindRenderPass(m_pRenderPass);

	RHICommandBuffer* cmd = context->GetCommandBuffer();
	cmd->BeginScopePass("BindFont");
	{
		ImGui_ImplVulkan_CreateFontsTexture(*static_cast<Vulkan::VulkanCommandBuffer*>(cmd)->GetHandle());
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.pFence = nullptr;
		submitDescriptor.waitSemaphoreCount = 0;
		submitDescriptor.pWaitSemaphores = nullptr;
		submitDescriptor.signalSemaphoreCount = 0;
		submitDescriptor.pSignalSemaphores = nullptr;
		submitDescriptor.waitStage = PIPELINE_STAGE_ALL_COMMANDS;
	}
	context->Submit(&submitDescriptor);
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}

void DrawGUIPass::Execute(RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{

	RHICommandBuffer* cmd = context->GetCommandBuffer();

	cmd->BeginScopePass("GUI");
	{
		RHIGraphicsEncoder *encoder = cmd->GetGraphicsEncoder();
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[ScriptableRenderPipeline::g_currentImage];
		}
		encoder->BeginPass(&renderPassBeginDescriptor);
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImDrawData* data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(data, *static_cast<Vulkan::VulkanCommandBuffer*>(cmd)->GetHandle(), nullptr);
		encoder->EndPass();
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = 1;
		submitDescriptor.pWaitSemaphores = &waitSemaphore;
		submitDescriptor.signalSemaphoreCount = 1;
		submitDescriptor.pSignalSemaphores = &signalSemaphore;
		submitDescriptor.pFence = nullptr;
		submitDescriptor.waitStage = PIPELINE_STAGE_VERTEX_INPUT;
	}
	context->Submit(&submitDescriptor);

	cmd->Clear();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}
