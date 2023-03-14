#include "pch.h"
#include "Framework/Public/Layer.h"
#include "Render/Public/Scene.h"
#include "Render/Public/SceneView.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Components/Public/CameraComponent.h"
#include "Render/Public/Viewport.h"
#include "Render/Renderer/Public/DeferredRenderer.h"

namespace WEngine
{

	Layer::Layer(const WString& name)
		: DebugName(name)
	{
		Scene = new RScene();
		GameObject *CameraGo = new GameObject(name + "_Camera");
		CameraGo->AddComponent<TransformComponent>();
		Camera = CameraGo->AddComponent<CameraComponent>();
	}

	void Layer::StartFrame()
	{
		Viewport->BeginRenderFrame();
	}

	void Layer::EndFrame()
	{
		GetRenderCommandList()->EndDrawingViewport(Viewport->GetRHI(), true);
	}

	void Layer::StartRendering()
	{
		WSceneViewFamily ViewFamily(Viewport.Get(), Scene.Get());

		SceneRenderer *Renderer = SceneRenderer::CreateRenderer(&ViewFamily);

		Renderer->Render();
	}

}
