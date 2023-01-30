#include "pch.h"
//#include "Render/Passes/Public/DrawGUIPass.h"
//#include "Render/Passes/Public/DrawSkyboxPass.h"
//#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
//#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
//#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
//#include "RHI/Public/RHIHeads.h"
//#include "Utils/Public/Window.h"
//#include "Utils/ImGui/Public/Gui.h"
//#include "Utils/Public/Synchronizer.h"
//#include "Platform/Vulkan/Public/VulkanCommandBuffer.h"
//#include "Platform/Vulkan/Public/VulkanDevice.h"
//#include "Scene/Public/GameObject.h"
//
//#include "deps/imgui/imgui_internal.h"
//
//DrawGUIPass::DrawGUIPass(ScriptableRenderer* pRenderer)
//	: ScriptableRenderPass(pRenderer)
//{
//	RHIAttachmentDescriptor attachmentDescriptors[] =
//	{
//		{ Format::B8G8R8A8_UNorm, 1, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLoadOP::Clear, AttachmentStoreOP::Store, AttachmentLayout::Undefined, AttachmentLayout::Present },
//	};
//	SubPassAttachment subpassColorAttachment = { 0, AttachmentLayout::ColorBuffer };
//	RHISubPassDescriptor subpassDescriptors = {};
//	{
//		subpassDescriptors.colorAttachmentCount = 1;
//		subpassDescriptors.pColorAttachments = &subpassColorAttachment;
//	}
//
//	RHISubPassDependencyDescriptor dependencyDescriptor[] =
//	{
//		{ -1, PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE, 0, PIPELINE_STAGE_EARLY_FRAGMENT_TESTS | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT | PIPELINE_STAGE_FRAGMENT_SHADER, ACCESS_COLOR_ATTACHMENT_READ | ACCESS_COLOR_ATTACHMENT_WRITE | ACCESS_DEPTH_STENCIL_ATTACHMENT_READ | ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE },
//	};
//
//	RHIRenderPassDescriptor renderPassDescriptor = {};
//	{
//		renderPassDescriptor.attachmentCount = 1;
//		renderPassDescriptor.pAttachmentDescriptors = attachmentDescriptors;
//		renderPassDescriptor.subpassCount = 1;
//		renderPassDescriptor.pSubPassDescriptors = &subpassDescriptors;
//		renderPassDescriptor.dependencyCount = 1;
//		renderPassDescriptor.pDependencyDescriptors = dependencyDescriptor;
//	}
//	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);
//	Gui::g_pGui->BindRenderPass(m_pRenderPass);
//}
//
//DrawGUIPass::~DrawGUIPass()
//{
//}
//
//void DrawGUIPass::Setup(RHIContext* context, CameraInfo* cameraData)
//{
//
//
//	m_pRenderTargets.Resize(3);
//	for (int i = 0; i < 3; ++i)
//	{
//		WEngine::WArray<RHITextureView*> textureViews = { context->GetTextureView(i) };
//		RHIRenderTargetDescriptor renderTargetDescriptor = {};
//		{
//			renderTargetDescriptor.bufferCount = 1;
//			renderTargetDescriptor.pBufferView = textureViews.GetData();
//			renderTargetDescriptor.renderPass = m_pRenderPass;
//			renderTargetDescriptor.width = Window::cur_window->GetWidth();
//			renderTargetDescriptor.height = Window::cur_window->GetHeight();
//		}
//		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
//	}
//
//
//	RHICommandBuffer* cmd = context->GetCommandBuffer(false);
//	cmd->BeginScopePass("BindFont");
//	{
//		ImGui_ImplVulkan_CreateFontsTexture(*static_cast<Vulkan::VulkanCommandBuffer*>(cmd)->GetHandle());
//	}
//	cmd->EndScopePass();
//	context->ExecuteCommandBuffer(cmd);
//	RHISubmitDescriptor submitDescriptor = {};
//	{
//		submitDescriptor.pFence = nullptr;
//		submitDescriptor.waitSemaphoreCount = 0;
//		submitDescriptor.pWaitSemaphores = nullptr;
//		submitDescriptor.signalSemaphoreCount = 0;
//		submitDescriptor.pSignalSemaphores = nullptr;
//		submitDescriptor.waitStage = PIPELINE_STAGE_ALL_COMMANDS;
//	}
//	context->Submit(&submitDescriptor);
//	cmd->~RHICommandBuffer();
//	WEngine::Allocator::Get()->Deallocate(cmd);
//
//	m_pCommandBuffers = context->GetCommandBuffer(RHIContext::g_maxFrames, false);
//
//}
//
//void DrawGUIPass::Execute(RHIContext* context, CameraInfo* cameraData)
//{
//	if (context->IsDisplayChanged())
//	{
//		for (int i = 0; i < 3; ++i)
//		{
//			WEngine::WArray<RHITextureView*> textureViews = { context->GetTextureView(i) };
//			RHIRenderTargetDescriptor renderTargetDescriptor = {};
//			{
//				renderTargetDescriptor.bufferCount = 1;
//				renderTargetDescriptor.pBufferView = textureViews.GetData();
//				renderTargetDescriptor.renderPass = m_pRenderPass;
//				renderTargetDescriptor.width = Window::cur_window->GetWidth();
//				renderTargetDescriptor.height = Window::cur_window->GetHeight();
//			}
//			delete m_pRenderTargets[i];
//			m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
//		}
//		context->ResetDisplayState();
//	}
//
//	RHICommandBuffer* cmd = m_pCommandBuffers[RHIContext::g_currentFrame];
//
//	cmd->BeginScopePass("Gui", m_pRenderPass, 0, m_pRenderTargets[RHIContext::g_currentImage]);
//	{
//		RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
//
//		ClearValue values[]{ {glm::vec4(1.f, 1.f, 1.f, 1.f), true } };
//		RHIRenderPassBeginDescriptor renderPassBeginDescriptor = {};
//		{
//			renderPassBeginDescriptor.renderPass = m_pRenderPass;
//			renderPassBeginDescriptor.renderTarget = m_pRenderTargets[RHIContext::g_currentImage];
//			renderPassBeginDescriptor.clearCount = 1;
//			renderPassBeginDescriptor.pClearValues = values;
//		}
//		encoder->BeginPass(&renderPassBeginDescriptor);
//
//		Gui::g_pGui->RenderGUI(cmd);
//
//		encoder->EndPass();
//		encoder->~RHIGraphicsEncoder();
//		WEngine::Allocator::Get()->Deallocate(encoder);
//	}
//	cmd->EndScopePass();
//	context->ExecuteCommandBuffer(cmd);
//
//	RHISemaphore *signals[] = { context->GetPresentVailableSemaphore() };
//	WEngine::WArray<WEngine::Trigger*> waitingSemaphores = WEngine::Synchronizer::GetTrigger("Gui");
//	WEngine::WArray<RHISemaphore*> waits;
//	waits.Resize(waitingSemaphores.Size());
//	for(int i = 0; i < waitingSemaphores.Size(); ++i)
//	{
//		waits[i] = waitingSemaphores[i]->signal;
//	}
//	waits.Push(context->GetImageVailableSemaphore());
//
//	RHISubmitDescriptor submitDescriptor = {};
//	{
//		submitDescriptor.waitSemaphoreCount = waits.Size();
//		submitDescriptor.pWaitSemaphores = waits.GetData();
//		submitDescriptor.signalSemaphoreCount = 1;
//		submitDescriptor.pSignalSemaphores = signals;
//		submitDescriptor.waitStage = PIPELINE_STAGE_FRAGMENT_SHADER | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;
//		submitDescriptor.pFence = context->GetFence();
//	}
//	context->Submit(&submitDescriptor);
//}
//
//void DrawGUIPass::UpdateRenderTarget(CameraInfo* cameraData)
//{
//	for (int i = 0; i < 3; ++i)
//	{
//		WEngine::WArray<RHITextureView*> textureViews = { RHIContext::GetContext()->GetTextureView(i) };
//		RHIRenderTargetDescriptor renderTargetDescriptor = {};
//		{
//			renderTargetDescriptor.bufferCount = 1;
//			renderTargetDescriptor.pBufferView = textureViews.GetData();
//			renderTargetDescriptor.renderPass = m_pRenderPass;
//			renderTargetDescriptor.width = Window::cur_window->GetWidth();
//			renderTargetDescriptor.height = Window::cur_window->GetHeight();
//		}
//		delete m_pRenderTargets[i];
//		m_pRenderTargets[i] = m_pDevice->CreateRenderTarget(&renderTargetDescriptor);
//	}
//}
