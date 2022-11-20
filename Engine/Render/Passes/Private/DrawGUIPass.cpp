#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/Passes/Public/DrawSkyboxPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Public/RHIHeads.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"
#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
#include "Platform/Vulkan/Public/VulkanDevice.h"
#include "Scene/Public/GameObject.h"

#include "deps/imgui/imgui_internal.h"

DrawGUIPass::DrawGUIPass()
{
	RHIAttachmentDescriptor attachmentDescriptors[] =
	{
		{ Format::B8G8R8A8_UNorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::Present },
	};
	SubPassAttachment subpassColorAttachment = { 0, AttachmentLayout::ColorBuffer };
	RHISubPassDescriptor subpassDescriptors = {};
	{
		subpassDescriptors.colorAttachmentCount = 1;
		subpassDescriptors.pColorAttachments = &subpassColorAttachment;
		subpassDescriptors.dependedStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptors.waitingStage = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		subpassDescriptors.waitingAccess = ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE;
	}
	RHIRenderPassDescriptor renderPassDescriptor = {};
	{
		renderPassDescriptor.attachmentCount = 1;
		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
		renderPassDescriptor.subpassCount = 1;
		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptors;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);
	Gui::g_pGui->BindRenderPass(m_pRenderPass);
}

DrawGUIPass::~DrawGUIPass()
{
}

void DrawGUIPass::Setup(RHIContext* context, CameraData* cameraData)
{


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


	RHICommandBuffer* cmd = context->GetCommandBuffer(false);
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

	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);

}

void DrawGUIPass::Execute(RHIContext* context, CameraData* cameraData)
{
	if (context->IsDisplayChanged())
	{
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
			delete m_pRenderTargets[i];
			m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
		}
		context->ResetDisplayState();
	}

	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];

	cmd->BeginScopePass("Test", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
	{
		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();

		ClearValue values[]{ {glm::vec4(1.f, 1.f, 1.f, 1.f), 0.0f, 0 } };
		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
		{
			renderPassBeginDescriptor.renderPass = m_pRenderPass;
			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentImage];
			renderPassBeginDescriptor.clearCount = 1;
			renderPassBeginDescriptor.pClearValues = values;
		}
		encoder->BeginPass(&renderPassBeginDescriptor);

		Gui::g_pGui->RenderGUI(cmd);

		encoder->EndPass();
		encoder->~RHIGraphicsEncoder();
		WEngine::Allocator::Get()->Deallocate(encoder);
	}
	cmd->EndScopePass();
	context->ExecuteCommandBuffer(cmd);

	RHISemaphore *signals[] = { context->GetPresentVailableSemaphore() };
	std::vector<WEngine::Trigger*> waitingSemaphores = WEngine::Synchronizer::GetTrigger("Gui");
	std::vector<RHISemaphore*> waits;
	waits.resize(waitingSemaphores.size());
	for(int i = 0; i < waitingSemaphores.size(); ++i)
	{
		waits[i] = waitingSemaphores[i]->signal;
	}
	waits.push_back(context->GetImageVailableSemaphore());

	RHISubmitDescriptor submitDescriptor = {};
	{
		submitDescriptor.waitSemaphoreCount = waits.size();
		submitDescriptor.pWaitSemaphores = waits.data();
		submitDescriptor.signalSemaphoreCount = 1;
		submitDescriptor.pSignalSemaphores = signals;
		submitDescriptor.waitStage = PIPELINE_STAGE_FRAGMENT_SHADER | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
		submitDescriptor.pFence = context->GetFence();
	}
	context->Submit(&submitDescriptor);
}

void DrawGUIPass::UpdateRenderTarget(CameraData* cameraData)
{
	for (int i = 0; i < 3; ++i)
	{
		std::vector<RHITextureView*> textureViews = { RHIContext::GetContext()->GetTextureView(i) };
		RHIRenderTargetDescriptor renderTargetDescriptor = {};
		{
			renderTargetDescriptor.bufferCount = 1;
			renderTargetDescriptor.pBufferView = textureViews.data();
			renderTargetDescriptor.renderPass = m_pRenderPass;
			renderTargetDescriptor.width = Window::cur_window->GetWidth();
			renderTargetDescriptor.height = Window::cur_window->GetHeight();
		}
		delete m_pRenderTargets[i];
		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
	}
}
