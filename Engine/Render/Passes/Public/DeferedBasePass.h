#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DeferedBasePass : public ScriptableRenderPass
{
public:

	DeferedBasePass(ScriptableRenderer *pRenderer);

	virtual ~DeferedBasePass();

	virtual void Setup() override;

	virtual void Execute() override;

};