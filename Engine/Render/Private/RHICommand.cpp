#include "pch.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"

void RHICommandBeginRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHIBeginRenderPass)(RenderPassDescriptor, FramebufferDescriptor);
}

void RHICommandEndRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHIEndRenderPass)();
}

void RHICommandBindVertexBuffer::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHIBindVertexBuffer)(VertexFactory);
}

void RHICommandSetStreamResource::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHISetStreamResource)(Stream);
}

void RHICommandBindIndexBuffer::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHIBindIndexBuffer)(IndexBuffer);
}

void RHICommandDrawIndexedPrimitive::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHIDrawIndexedPrimitive)(indexCount, firstIndex, instanceCount);
}

void RHICommandSetViewport::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHISetViewport)(MinX, MinY, MaxX - MinX, MaxY - MaxY, MinZ, MaxZ);
}

void RHICommandSetScissor::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(RHISetScissor)(OffsetX, OffsetY, Width, Height);
}