#pragma once
#include "Render/RenderPipeline/public/ScriptableRenderPipeline.h"

class DrawGUIPass;

namespace WEngine
{

	class GuiRenderPipeline : public ScriptableRenderPipeline
	{
	public:

		GuiRenderPipeline(RHIContext *pContext);

		virtual ~GuiRenderPipeline();

		virtual void Init() override;

		virtual void Setup() override;

		virtual void Execute() override;

	private:

		DrawGUIPass *m_pPass;

	};

}