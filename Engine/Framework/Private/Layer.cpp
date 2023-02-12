#include "pch.h"
#include "Framework/Public/Layer.h"
#include "Render/Public/Scene.h"

namespace WEngine
{

	Layer::Layer(const WString& name)
		: DebugName(name)
	{
		Scene = new RScene();
	}

}
