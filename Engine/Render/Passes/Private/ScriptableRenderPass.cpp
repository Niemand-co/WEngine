#include "pch.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"
#include "RHI/Public/RHIGroup.h"

ScriptableRenderPass::ScriptableRenderPass(RenderPassConfigure* configure)
	: m_pRenderPass(configure->pRenderPass), m_pPSO(configure->pPSO), m_pDevice(configure->pDevice)
{
}

ScriptableRenderPass::~ScriptableRenderPass()
{
}

RHIGroup* ScriptableRenderPass::GetResourceGroup()
{
	return m_pResource;
}
