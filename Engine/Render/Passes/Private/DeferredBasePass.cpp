#include "pch.h"
#include "..\Public\DeferredBasePass.h"

DeferredBasePass::DeferredBasePass(ScriptableRenderer* pRenderer)
	: ScriptableRenderPass(pRenderer)
{
	Setup();
}

DeferredBasePass::~DeferredBasePass()
{
}

void DeferredBasePass::Setup()
{
}

void DeferredBasePass::Execute()
{
	
}
