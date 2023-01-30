#include "pch.h"
#include "Render/RenderPipeline/Public/GameRenderPipeline.h"
#include "Render/Public/ScriptableRenderer.h"
#include "Scene/Components/Public/CameraComponent.h"

namespace WEngine
{

	GameRenderPipeline::GameRenderPipeline(RHIContext *pContext)
		: ScriptableRenderPipeline(pContext)
	{
	}

	GameRenderPipeline::~GameRenderPipeline()
	{
	}

	void GameRenderPipeline::Init()
	{
	}

	void GameRenderPipeline::Setup()
	{
		ScriptableRenderPipeline::Setup();
	}

	void GameRenderPipeline::Execute()
	{
		for (unsigned int i = 0; i < m_pCameras.Size(); ++i)
		{
			m_pCameras[i]->GetRenderer()->Execute(m_pContext, m_pCameras[i]->GetCameraInfo());
		}
	}

}