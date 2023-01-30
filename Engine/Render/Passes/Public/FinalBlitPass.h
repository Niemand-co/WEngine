#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class FinalBlitPass : public ScriptableRenderPass
{
public:

	FinalBlitPass(ScriptableRenderer* pRenderer);

	virtual ~FinalBlitPass();

	virtual void Setup() override;

	virtual void Execute() override;

};