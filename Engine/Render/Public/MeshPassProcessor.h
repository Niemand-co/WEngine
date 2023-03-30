#pragma once
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineResourceLayoutDescriptor.h"
#include "Render/Descriptor/Public/RHIRasterizationDescriptor.h"

class WMeshBatch;
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

	uint32 RenderTargetCount = 0;

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

struct WMeshDrawShaderBindings
{
public:

	WMeshDrawShaderBindings() = default;

	~WMeshDrawShaderBindings() = default;

	void Initialize(const WMeshPassProcessorShaderBase *Shaders);

private:

	WEngine::WArray<WShaderParameterLayout> Layouts;

};

class WMeshDrawCommand
{
public:

	WMeshDrawCommand() {}
	WMeshDrawCommand(WMeshDrawCommand&& Other) = default;
	WMeshDrawCommand(const WMeshDrawCommand& Other) = default;
	WMeshDrawCommand& operator=(WMeshDrawCommand&& Other) = default;
	WMeshDrawCommand& operator=(const WMeshDrawCommand& Other) = default;

	void SetParameters(const WMeshBatch& MeshBatch, uint32 MeshBatchElementIndex, const WMeshPassProcessorShaderBase *Shaders, const class RHIGraphicsPipelineStateDescriptor& InPipelineDescriptor);

	void SubmitDrawBegin(WRenderPassRHIRef RenderPass);

	void SubmitDrawEnd();

private:

	WIndexBufferRHIRef IndexBuffer;

	uint32 FirstIndex;

	uint32 NumPrimitives;

	uint32 NumInstances;

	RHIGraphicsPipelineStateDescriptor PipelineDescriptor;

	WMeshDrawShaderBindings ShaderBindings;

};