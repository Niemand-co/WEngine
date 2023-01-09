#pragma once

class RHICommandListBase;

class RHICommandBase
{
	virtual void ExecuteAndDestruct(RHICommandListBase *CmdList) = 0;

	RHICommandBase *next;
};

template<typename T>
struct RHICommand
{
	virtual void ExecuteAndDestruct(RHICommandListBase *CmdList) override final
	{
		T* cmd = static_cast<T*>(this);
		cmd->Execute(CmdList);
		cmd->~T();
	}
};

struct RHICommandBeginRenderPass final : public RHICommand<RHICommandBeginRenderPass>
{
	class RHIRenderPassBeginDescriptor* descriptor;

	RHICommandBeginRenderPass(RHIRenderPassBeginDescriptor* inDescriptor)
		: descriptor(inDescriptor)
	{

	}

	void Execute(RHICommandListBase *CmdList);

};

struct RHICommandEndRenderPass final : public RHICommand<RHICommandEndRenderPass>
{
	void Execute(RHICommandListBase *CmdList);
};