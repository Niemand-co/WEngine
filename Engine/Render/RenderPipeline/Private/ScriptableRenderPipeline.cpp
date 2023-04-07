#include "pch.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/RenderPassHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "RHI/Public/RHIGPU.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "Platform/Vulkan/Public/VulkanInstance.h"
#include "Utils/Public/Window.h"
#include "Scene/Public/World.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Utils/ImGui/Public/Gui.h"

ScriptableRenderPipeline::ScriptableRenderPipeline(RHIContext *pContext)
	: m_pContext(pContext)
{
}

ScriptableRenderPipeline::~ScriptableRenderPipeline()
{
}

void ScriptableRenderPipeline::Init()
{
}

void ScriptableRenderPipeline::Setup()
{

}

void ScriptableRenderPipeline::Execute()
{

}

ScriptableRenderer* ScriptableRenderPipeline::CreateRenderer()
{
	return nullptr;
}