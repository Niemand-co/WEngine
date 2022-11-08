#include "pch.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "Render/Passes/Public/DrawGUIPass.h"

namespace WEngine
{

	GuiRenderPipeline::GuiRenderPipeline(RHIContext *pContext)
		: ScriptableRenderPipeline(pContext)
	{
		m_pPass = new DrawGUIPass();
	}

	GuiRenderPipeline::~GuiRenderPipeline()
	{

	}

	void GuiRenderPipeline::Init()
	{
	}

	void GuiRenderPipeline::Setup()
	{
		m_pPass->Setup(m_pContext, nullptr);
	}

	void GuiRenderPipeline::Execute()
	{
		m_pPass->Execute(m_pContext, nullptr);
	}

}