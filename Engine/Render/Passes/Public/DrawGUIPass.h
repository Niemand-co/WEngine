#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIPipelineStateObject;
class RHISampler;
class GameObject;

class DrawGUIPass : public ScriptableRenderPass
{
public:

	DrawGUIPass(ScriptableRenderer* pRenderer);

	virtual ~DrawGUIPass();

	virtual void Setup() override;

	virtual void Execute() override;

private:

	GameObject *m_currentGo;

	RHISampler *m_pSampler;

};