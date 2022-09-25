#include "pch.h"
#include "Framework/Public/Application.h"
#include "Utils/Public/Window.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Descriptor/Public/RHISwapchainDescriptor.h"
#include "Render/Descriptor/Public/RHITextureViewDescriptor.h"
#include "Render/Descriptor/Public/RHIShaderDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderPassDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderTargetDescriptor.h"
#include "Render/Descriptor/Public/RHIRenderPassBeginDescriptor.h"
#include "RHI/Public/RHIQueue.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHITexture.h"
#include "RHI/Public/RHIFence.h"
#include "RHI/Public/RHICommandPool.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIShader.h";
#include "RHI/Public/RHIPipelineStateObject.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "Platform/Vulkan/Public/VulkanMacro.h"

Application* Application::m_instance = nullptr;

Application::Application()
{
	if(m_instance != nullptr)return;
	m_instance = this;
}

Application::~Application()
{

}

Application* Application::CreateApplication()
{
	if (m_instance == nullptr)
	{
		return new Application();
	}
	else
	{
		std::cout<<"Over Created Application"<<std::endl;
		exit(0);
	}
}

void Application::Init()
{
	m_allocator = new Allocator();

	WinProc proc = { "WEngine", 1920u, 1080u };
	m_window = Window::Get(&proc);

	m_pipeline = ScriptableRenderPipeline::get();

	m_pipeline->AddRenderer();

}

void Application::Tick()
{
	while (!IsQuit())
	{
		m_window->Update();
		if(m_window->GetIsClosed())
			m_isQuit = true;
		m_pipeline->Execute();
	}
	Finalize();
}

bool Application::IsQuit()
{
	return m_isQuit;
}

void Application::Finalize()
{
}
