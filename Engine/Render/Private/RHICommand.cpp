#include "pch.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Public/RHIHeads.h"
#include "Render/Descriptor/Public/RHIDescriptorHeads.h"
#include "RHI/Encoder/Public/RHIGraphicsEncoder.h"
#include "RHI/Encoder/Public/RHIComputeEncoder.h"

#define ENCODER_EXECUTE(command) static_cast<RHIRenderCommandList*>(CmdList)->GetEncoder()->command

void RHICommandBeginRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(BeginPass)(descriptor);
}

void RHICommandEndRenderPass::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(EndPass)();
}

void RHICommandDrawIndexedPrimitive::Execute(RHICommandListBase* CmdList)
{
	ENCODER_EXECUTE(DrawIndexed)(indexCount, firstIndex, instanceCount);
}

void RHICommandSetViewport::Execute(RHICommandListBase* CmdList)
{
	RHIViewportDescriptor descriptor = {};
	{
		descriptor.x = MinX;
		descriptor.y = MinY;
		descriptor.width = MaxX - MinX;
		descriptor.height = MaxY - MinY;
		descriptor.minDepth = MinZ;
		descriptor.maxDepth = MinZ == MaxZ ? (MinZ + 1.0f) : MaxZ;
	}
	//RHIContext::CreateViewport(&descriptor);
	
	//ENCODER_EXECUTE(SetViewport)(viewport);
}

void RHICommandSetScissor::Execute(RHICommandListBase* CmdList)
{
}
