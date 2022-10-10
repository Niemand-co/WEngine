#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Public/RHIRenderTarget.h"
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
	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.attachmentFormat = Format::A16R16G16B16_SFloat;
		renderPassDescriptor.attachmentLoadOP = AttachmentLoadOP::Load;
		renderPassDescriptor.attachmentStoreOP = AttachmentStoreOP::Store;
		renderPassDescriptor.sampleCount = 1;
		renderPassDescriptor.stencilLoadOP = AttachmentLoadOP::Load;
		renderPassDescriptor.stencilStoreOP = AttachmentStoreOP::Store;
		renderPassDescriptor.initialLayout = AttachmentLayout::General;
		renderPassDescriptor.finalLayout = AttachmentLayout::Present;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);

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
	context->Submit(nullptr, nullptr, nullptr);
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
	context->Submit(waitSemaphore, signalSemaphore, fence);

	cmd->Clear();
	cmd->~RHICommandBuffer();
	WEngine::Allocator::Get()->Deallocate(cmd);
}
