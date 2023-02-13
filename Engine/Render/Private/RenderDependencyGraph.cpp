#include "pch.h"
#include "Render/Public/RenderDependencyGraph.h"

WEngine::Allocator<6>* WEngine::Allocator<6>::g_pInstance = WRDGAllocator();