#pragma once

#define ENCODER_EXECUTE(command) GetDynamicRHI()->command

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

struct RHICommandBindVertexBuffer final : public RHICommand<RHICommandBindVertexBuffer>
{
	WVertexFactory* VertexFactory;

	RHICommandBindVertexBuffer(WVertexFactory* InVertexFactory)
		: VertexFactory(InVertexFactory)
	{
	}

	void Execute(RHICommandListBase *CmdList);
};

struct RHICommandSetStreamResource final : public RHICommand<RHICommandSetStreamResource>
{
	VertexInputStream Stream;

	RHICommandSetStreamResource(const VertexInputStream& InStream)
		: Stream(InStream)
	{
	}

	void Execute(RHICommandListBase *CmdList);
};

struct RHICommandBindIndexBuffer final : public RHICommand<RHICommandBindIndexBuffer>
{
	WIndexBufferRHIRef IndexBuffer;

	RHICommandBindIndexBuffer(WIndexBufferRHIRef InIndexBuffer)
		: IndexBuffer(InIndexBuffer)
	{
	}

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
	int32 OffsetX;
	int32 OffsetY;
	uint32 Width;
	uint32 Height;

	RHICommandSetScissor(int32 InOffsetX, int32 InOffsetY, uint32 InWidth, uint32 InHeight)
		: OffsetX(InOffsetX), OffsetY(InOffsetY), Width(InWidth), Height(InHeight)
	{
	}

	void Execute(RHICommandListBase* CmdList);
};