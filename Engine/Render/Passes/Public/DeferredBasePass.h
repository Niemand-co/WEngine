#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DeferredBasePass : public ScriptableRenderPass
{
public:

	DeferredBasePass(ScriptableRenderer *pRenderer);

	virtual ~DeferredBasePass();

	virtual void Setup() override;

	virtual void Execute() override;

};