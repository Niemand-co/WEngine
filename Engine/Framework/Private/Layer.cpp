#include "pch.h"
#include "Framework/Public/Layer.h"
#include "Render/Public/Scene.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Components/Public/CameraComponent.h"

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

	void Layer::StartRendering()
	{
		Camera->GetRenderer()->Render(Viewport);
	}

}
