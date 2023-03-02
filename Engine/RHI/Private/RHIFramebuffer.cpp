#include "pch.h"
#include "RHI/Public/RHIFramebuffer.h"

WEngine::WHashMap<uint32, WFramebufferRHIRef> RHIFramebufferManager::Framebuffers = WEngine::WHashMap<uint32, WFramebufferRHIRef>();