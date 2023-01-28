#pragma once
#include "Framework/Public/Layer.h"

class RHITexture;
class RHISampler;

namespace WEngine
{

	class TimeStep;
	class GuiRenderPipeline;
	class SceneHierarchyPanel;
	class InspectorPanel;

	class GuiLayer : public Layer
	{
	public:

		GuiLayer(const WString& name);

		virtual ~GuiLayer();

		virtual void OnAttach() override;

		virtual void OnDettach() override;

		virtual void OnEvent(Event* pEvent) override;

		virtual void Tick(TimeStep timeStep) override;

	private:

		RHISampler *m_pSampler;

		RHITexture *m_pTexture;

		WArray<ImTextureID> m_imageID;

		WPair<ImVec2, ImVec2> m_displayArea;

		InspectorPanel *m_pInspectorPanel;

		bool m_isHierarchyShowed;

		SceneHierarchyPanel *m_pHierarchyPanel;

		bool m_isConsoleShowed;

		bool m_isSettingsShowed;

		ImFont *m_pOblique;

	};

}