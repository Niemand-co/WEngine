#include "pch.h"
#include "Render/RenderPipeline/Public/GuiRenderPipeline.h"
#include "Render/Passes/Public/DrawGUIPass.h"

namespace WEngine
{

	GuiRenderPipeline::GuiRenderPipeline(RHIContext *pContext)
		: ScriptableRenderPipeline(pContext)
	{
	}

	GuiRenderPipeline::~GuiRenderPipeline()
	{

	}

	void GuiRenderPipeline::Init()
	{
	}

	void GuiRenderPipeline::Setup()
	{
		m_pPass->Setup();
	}

	void GuiRenderPipeline::Execute()
	{
		m_pPass->Execute();
	}

}