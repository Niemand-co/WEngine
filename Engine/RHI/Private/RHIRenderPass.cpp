#include "pch.h"
#include "RHI/Public/RHIRenderPass.h"

WEngine::WHashMap<uint32, WRenderPassRHIRef> RHIRenderPassManager::RenderPasses = WEngine::WHashMap<uint32, WRenderPassRHIRef>();