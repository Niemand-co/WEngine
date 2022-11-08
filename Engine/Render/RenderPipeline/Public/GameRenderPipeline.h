#pragma once
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"

class RHIContext;

namespace WEngine
{
	
	class GameRenderPipeline : public ScriptableRenderPipeline
	{
	public:
		
		GameRenderPipeline(RHIContext *pContext);

		virtual ~GameRenderPipeline();

		virtual void Init();

		virtual void Setup();

		virtual void Execute();

	};

}