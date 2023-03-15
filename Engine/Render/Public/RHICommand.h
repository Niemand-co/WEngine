#pragma once

#define ENCODER_EXECUTE(command) static_cast<RHIRenderCommandList*>(CmdList)->command

struct RHICommandListBase;

struct RHICommandBase
{
	virtual void ExecuteAndDestruct(RHICommandListBase *CmdList) = 0;

	RHICommandBase *next = nullptr;
};

template<typename T>
struct RHICommand : public RHICommandBase
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
	class RHIRenderPassDescriptor* RenderPassDescriptor;
	class RHIFramebufferDescriptor* FramebufferDescriptor;

	RHICommandBeginRenderPass(RHIRenderPassDescriptor* inRenderPassDescriptor, RHIFramebufferDescriptor *inFramebufferDescriptor)
		: RenderPassDescriptor(inRenderPassDescriptor),
		  FramebufferDescriptor(inFramebufferDescriptor)
	{

	}

	void Execute(RHICommandListBase *CmdList);

};

struct RHICommandEndRenderPass final : public RHICommand<RHICommandEndRenderPass>
{
	void Execute(RHICommandListBase *CmdList);
};

struct RHICommandDrawIndexedPrimitive final : public RHICommand<RHICommandDrawIndexedPrimitive>
{
	unsigned int indexCount;
	unsigned int firstIndex;
	unsigned int instanceCount;

	RHICommandDrawIndexedPrimitive(unsigned int inIndexCount, unsigned int inFirstIndex, unsigned int inInstanceCount)
		: indexCount(inIndexCount), firstIndex(inFirstIndex), instanceCount(inInstanceCount)
	{

	}

	void Execute(RHICommandListBase *CmdList);
};

struct RHICommandSetViewport final : public RHICommand<RHICommandSetViewport>
{
	float MinX;
	float MinY;
	float MinZ;
	float MaxX;
	float MaxY;
	float MaxZ;

	RHICommandSetViewport(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
		: MinX(minX), MaxX(maxX), MinY(minY), MaxY(maxY), MinZ(minZ), MaxZ(maxZ)
	{

	}

	void Execute(RHICommandListBase *CmdList);
};

struct RHICommandSetScissor final : public RHICommand<RHICommandSetScissor>
{
	unsigned int MinX;
	unsigned int MinY;
	unsigned int MaxX;
	unsigned int MaxY;

	RHICommandSetScissor(unsigned int minX, unsigned int maxX, unsigned int minY, unsigned int maxY)
		: MinX(minX), MaxX(maxX), MinY(minY), MaxY(maxY)
	{

	}

	void Execute(RHICommandListBase* CmdList);
};