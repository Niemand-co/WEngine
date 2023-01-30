#include "pch.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"

RHIRenderCommandList* RHIRenderCommandList::g_instance = nullptr;

RHIRenderCommandList::RHIRenderCommandList()
{
}

void RHIRenderCommandList::BeginRenderPass(RHIRenderPassBeginDescriptor* descriptor)
{
}
