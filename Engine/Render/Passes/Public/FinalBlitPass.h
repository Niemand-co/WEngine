#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class FinalBlitPass : public ScriptableRenderPass
{
public:

	FinalBlitPass(class ScriptableRenderer* pRenderer);

	virtual ~FinalBlitPass();

};