#include "pch.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"

ScriptableRenderPass::ScriptableRenderPass(RenderPassConfigure* configure)
	: m_pContext(configure->pContext), m_pRenderPass(configure->pRenderPass), m_pPSO(configure->pPSO), m_pDevice(configure->pDevice)
{
}

ScriptableRenderPass::~ScriptableRenderPass()
{
}
