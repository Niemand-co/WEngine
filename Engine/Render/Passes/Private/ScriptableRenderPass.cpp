#include "pch.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"
#include "RHI/Public/RHIGroup.h"
#include "RHI/Public/RHIContext.h"

ScriptableRenderPass::ScriptableRenderPass(ScriptableRenderer *pRenderer)
	: m_pRenderer(pRenderer)
{
	m_pDevice = RHIContext::GetDevice();
}

ScriptableRenderPass::~ScriptableRenderPass()
{
}

RHIGroup* ScriptableRenderPass::GetResourceGroup()
{
	return m_pResource;
}
