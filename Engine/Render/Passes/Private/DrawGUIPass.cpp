#include "pch.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Public/RHIHeads.h"
#include "Utils/Public/Window.h"
#include "Utils/ImGui/Public/Gui.h"

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
		renderPassDescriptor.stencilLoadOP = AttachmentLoadOP::Clear;
		renderPassDescriptor.stencilStoreOP = AttachmentStoreOP::DontCare;
	}
	m_pRenderPass = m_pDevice->CreateRenderPass(&renderPassDescriptor);
	
	Gui::g_pGui->BindRenderPass(m_pRenderPass);
}

void DrawGUIPass::Execute(RHIContext* context, RHISemaphore* waitSemaphore, RHISemaphore* signalSemaphore, RHIFence* fence)
{
	//RHICommandBuffer *cmd = context->GetCommandBuffer();
	//
	//cmd->BeginScopePass("Gui");
	//{
	//	RHIGraphicsEncoder* encoder = cmd->GetGraphicsEncoder();
	//}
	
}
