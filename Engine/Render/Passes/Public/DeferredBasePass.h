#pragma once
#include "Render/Passes/Public/ScriptableRenderPass.h"

class DeferredBasePass : public ScriptableRenderPass
{
public:

	DeferredBasePass();

	virtual ~DeferredBasePass();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};