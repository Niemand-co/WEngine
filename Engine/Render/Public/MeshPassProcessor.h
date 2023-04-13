#pragma once
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineResourceLayoutDescriptor.h"
#include "Render/Descriptor/Public/RHIRasterizationDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineStateObjectDescriptor.h"
#include "Render/Mesh/Public/MeshBatch.h"

class WMaterialShader;
struct MaterialProxy;

class WMeshPassProcessorRenderState
{
public:

	WMeshPassProcessorRenderState() = default;

	WMeshPassProcessorRenderState(WUniformBufferRHIRef InViewUniformBuffer, WUniformBufferRHIRef InPassUniformBuffer)
		: ViewUniformBuffer(InViewUniformBuffer),
		  PassUniformBuffer(InPassUniformBuffer)
	{
	}

	~WMeshPassProcessorRenderState() = default;

	void SetDepthStencilState(const WDepthStencilStateRHIRef& InDepthStencilState) { DepthStencilState = InDepthStencilState; }

	void SetBlendState(uint32 AttachmentIndex, const WBlendStateRHIRef& InBlendState) { BlendStates[AttachmentIndex] = InBlendState; }

	void SetRasterizationState(const WRasterizationStateRHIRef& InRasterizationState) { RasterizationState = InRasterizationState; }

	void SetMultiSampleState(const WMultiSampleStateRHIRef& InMultiSampleState) { MultiSampleState = InMultiSampleState; }

	const WDepthStencilStateRHIRef& GetDepthStencilState() const { return DepthStencilState; }

	const WBlendStateRHIRef& GetBlendState(uint32 AttachmentIndex) const { return BlendStates[AttachmentIndex]; }

	const WRasterizationStateRHIRef& GetRasterizationState() const { return RasterizationState; }

	const WMultiSampleStateRHIRef& GetMultiSamlpState() const { return MultiSampleState; }

private:

	WDepthStencilStateRHIRef DepthStencilState = nullptr;

	WBlendStateRHIRef BlendStates[MaxSimultaneousRenderTargets] = {0};

	WRasterizationStateRHIRef RasterizationState = nullptr;

	WMultiSampleStateRHIRef MultiSampleState = nullptr;

	WUniformBufferRHIRef ViewUniformBuffer = nullptr;

	WUniformBufferRHIRef PassUniformBuffer = nullptr;

	friend class WMeshPassProcessor;

};

class WMeshPassProcessor
{
public:

	WMeshPassProcessor(const class RScene* InScene, const struct WViewInfo* InView)
		: Scene(InScene),
		  View(InView)
	{
	}

	virtual ~WMeshPassProcessor() = default;

	void SetDrawListContext(class WMeshPassDrawListContext* InDrawList)
	{
		DrawList = InDrawList;
	}

	template<typename PassShaderType>
	void BuildMeshDrawCommand(
	const WMeshBatch& MeshBatch,
	const WMeshPassProcessorRenderState& RenderState,
	PassShaderType *PassShader,
	const MaterialProxy& Material,
	EPassFeature Feature);

protected:

	const RScene *Scene;

	const WViewInfo *View;

	WMeshPassDrawListContext *DrawList;

};

class WMeshPassProcessorShaderBase
{
public:

	virtual WVertexShaderRHIRef GetVertexShaderRHI() const = 0;

	virtual WGeometryShaderRHIRef GetGeometryShaderRHI() const = 0;

	virtual WPixelShaderRHIRef GetPixelShaderRHI() const = 0;

	virtual WMaterialShader* GetVertexShader() const = 0;

	virtual WMaterialShader* GetGeometryShader() const = 0;

	virtual WMaterialShader* GetPixelShader() const = 0;

};

template<typename VertexType, typename GeometryType, typename PixelType>
class WMeshPassProcessorShader : public WMeshPassProcessorShaderBase
{
public:

	WMeshPassProcessorShader() = default;

	~WMeshPassProcessorShader() = default;

	virtual WVertexShaderRHIRef GetVertexShaderRHI() const override { return VertexShader ? VertexShader->GetVertexShader() : nullptr; }

	virtual WGeometryShaderRHIRef GetGeometryShaderRHI() const override { return GeometryShader ? GeometryShader->GetGeometryShader() : nullptr; }

	virtual WPixelShaderRHIRef GetPixelShaderRHI() const override { return PixelShader ? PixelShader->GetPixelShader() : nullptr; }

	virtual WMaterialShader* GetVertexShader() const { return VertexShader; }

	virtual WMaterialShader* GetGeometryShader() const { return GeometryShader; } 

	virtual WMaterialShader* GetPixelShader() const { return PixelShader; }

public:

	VertexType *VertexShader = nullptr;

	GeometryType *GeometryShader = nullptr;

	PixelType *PixelShader = nullptr;

};

struct ShaderBindingSlot
{
	uint32 Slot;
	uint32 Count;
	EUniformBaseType Type;
};

struct WMeshDrawShaderBindings
{
public:

	WMeshDrawShaderBindings() = default;

	~WMeshDrawShaderBindings() = default;

	void Initialize(const WMeshPassProcessorShaderBase *Shaders);

	void Add(EUniformBaseType InType, uint32 InCount);

	template<typename LAMBDA>
	void EnumerateBindings(LAMBDA lambda) const
	{
		for (const ShaderBindingSlot& Binding : Bindings)
		{
			lambda(Binding);
		}
	}

	uint32 GetHashCode() const { return HashCode; }

private:

	WEngine::WArray<ShaderBindingSlot> Bindings;

	uint32 HashCode = 0;

};

class WMeshDrawCommand
{
public:

	WMeshDrawCommand() {}
	WMeshDrawCommand(WMeshDrawCommand&& Other) = default;
	WMeshDrawCommand(const WMeshDrawCommand& Other) = default;
	WMeshDrawCommand& operator=(WMeshDrawCommand&& Other) = default;
	WMeshDrawCommand& operator=(const WMeshDrawCommand& Other) = default;

	void SetParameters(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, VertexInputStream& Stream, const WMeshPassProcessorShaderBase *Shaders, const class RHIGraphicsPipelineStateDescriptor& InPipelineDescriptor);

	WMeshDrawShaderBindings& GetShaderBinding(uint32 ShaderStage);

	static bool SubmitDrawBegin(WMeshDrawCommand& Command, WRenderPassRHIRef RenderPass, RHIRenderCommandList& CmdList);

	static void SubmitDrawEnd(WMeshDrawCommand& Command, RHIRenderCommandList& CmdList);

	static void SubmitDrawCommand(WMeshDrawCommand& Command, WRenderPassRHIRef RenderPass, RHIRenderCommandList& CmdList)
	{
		if (SubmitDrawBegin(Command, RenderPass, CmdList))
		{
			SubmitDrawEnd(Command, CmdList);
		}
	}

private:

	VertexInputStream VertexStream;

	WIndexBufferRHIRef IndexBuffer = nullptr;

	uint32 FirstIndex = 0;

	uint32 NumPrimitives = 0;

	uint32 NumInstances = 0;

	RHIGraphicsPipelineStateDescriptor PipelineDescriptor;

	WMeshDrawShaderBindings ShaderBindings[MaxGraphicsPipelineShaderNum];

};

typedef WEngine::WArray<WMeshDrawCommand> WMeshCommandOneFrameArray;

class WMeshPassDrawListContext
{
public:

	virtual ~WMeshPassDrawListContext() = default;

	virtual WMeshDrawCommand& AddCommand() = 0;

	virtual void FinalizeCommand(RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass) = 0;

};

class WDynamicMeshPassDrawListContext : public WMeshPassDrawListContext
{
public:

	WDynamicMeshPassDrawListContext() = default;

	virtual ~WDynamicMeshPassDrawListContext() = default;

	virtual WMeshDrawCommand& AddCommand() override;

	virtual void FinalizeCommand(RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass) override;

private:

	WMeshCommandOneFrameArray MeshCommands;

};

class WCachedMeshPassDrawListContext : public WMeshPassDrawListContext
{
public:

	WCachedMeshPassDrawListContext();

	virtual ~WCachedMeshPassDrawListContext();

	virtual WMeshDrawCommand& AddCommand() override;

	virtual void FinalizeCommand(RHIRenderCommandList& CmdList, WRenderPassRHIRef RenderPass) override;

};