#pragma once
#include "Framework/Public/Layer.h"

class RHITexture;
class RHISampler;

namespace WEngine
{

	class TimeStep;
	class GuiRenderPipeline;

	class GuiLayer : public Layer
	{
	public:

		GuiLayer(std::string_view name);

		virtual ~GuiLayer();

		virtual void OnAttach() override;

		virtual void OnDettach() override;

		virtual void OnEvent(Event* pEvent) override;

		virtual void OnUpdate(TimeStep timeStep) override;

	private:

		RHISampler *m_pSampler;

		RHITexture *m_pTexture;

		ImTextureID m_imageID;

	};

}