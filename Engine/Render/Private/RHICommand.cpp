#include "pch.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"

void RHICommandBeginRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(BeginRenderPass)(descriptor);
}

void RHICommandEndRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(EndRenderPass)();
}

void RHICommandDrawIndexedPrimitive::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(DrawIndexedPrimitive)(indexCount, firstIndex, instanceCount);
}

void RHICommandSetViewport::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(SetViewport)({});
}

void RHICommandSetScissor::Execute(RHICommandListBase* CmdList)
{
}
