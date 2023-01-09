#include "pch.h"
#include "Render/Public/RHICommand.h"
#include "Render/Public/RHICommandList.h"
#include "RHI/Public/RHIHeads.h"

void RHICommandBeginRenderPass::Execute(RHICommandListBase* CmdList)
{
	CmdList->GetContext();
}

void RHICommandEndRenderPass::Execute(RHICommandListBase* CmdList)
{
}
