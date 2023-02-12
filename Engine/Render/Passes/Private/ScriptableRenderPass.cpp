#include "pch.h"
#include "Render/Passes/Public/ScriptableRenderPass.h"
#include "RHI/Public/RHIGroup.h"
#include "RHI/Public/RHIContext.h"

WEngine::WHashMap<int32, WRenderPassRHIRef> ScriptableRenderPass::Passes = WEngine::WHashMap<int32, WRenderPassRHIRef>();