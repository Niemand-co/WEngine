#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class RHIGroup;
class RHIBuffer;
class RHIPipelineResourceLayout;
class RHIBuffer;
class WStaticMesh;
class RHITexture;
class RHITextureView;

class DrawGizmosPass : public ScriptableRenderPass
{
public:

	DrawGizmosPass(ScriptableRenderer* pRenderer);

	~DrawGizmosPass();

private:

};