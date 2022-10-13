#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Public/RHIHeads.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanEvent.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"

DrawGUIPass::DrawGUIPass(RenderPassConfigure *pConfigure)
	: ScriptableRenderPass(pConfigure)
{

}

DrawGUIPass::~DrawGUIPass()
{
}

void DrawGUIPass::Setup(RHIContext* context, CameraData* cameraData)
{
	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::A16R16G16B16_SFloat, 1, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLoadOP::Load, AttachmentStoreOP::Store, AttachmentLayout::ColorBuffer, AttachmentLayout::Present },
	};
	RHISubPassDescriptor subpassDescriptors[] =
	{
		{ 0, AttachmentLayout::ColorBuffer, -1, PIPELINE_STAGE_BOTTOM_OF_PIPE, 0, PIPELINE_STAGE_TOP_OF_PIPE, ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
	};
	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = subpassDescriptors;
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
}

void DrawGUIPass::Execute(RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence, RHIEvent* pEvent)
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
		{
			ImGui::Begin("Inspector");
			//ImGui::SliderFloat("Roughness")
			ImGui::End();
		}
		Gui::g_pGui->RenderGUI(cmd);
		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);
	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = 1;
		submitDescriptor.pWaitSemaphores = &waitSemaphore;
		submitDescriptor.signalSemaphoreCount = 1;
		submitDescriptor.pSignalSemaphores = &signalSemaphore;
		submitDescriptor.pFence = fence;
		submitDescriptor.waitStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
	}
	context->Submit(&submitDescriptor);

	//cmd->Clear();
	//cmd->~RHICommandBuffer();
	//WEngine::Allocator::Get()->Deallocate(cmd);
}
