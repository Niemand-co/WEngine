#include "pch.h"
#include "Render/Renderer/Public/ScriptableRenderer.h"
#include "Render/Passes/Public/DrawOpaquePass.h"
#include "Render/Passes/Public/MainLightShadowPass.h"
#include "Render/Passes/Public/FinalBlitPass.h"
#include "Render/Passes/Public/DrawGUIPass.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Public/RHIHeads.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Utils/Public/Synchronizer.h"

ScriptableRenderer::ScriptableRenderer(CameraComponent* pCamera, WViewport* Viewport)
	: m_pCamera(pCamera),
	  Viewport(Viewport)
{
}

ScriptableRenderer::~ScriptableRenderer()
{
}