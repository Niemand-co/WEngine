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

	WMeshPassProcessor(const class RScene* InScene, const WViewInfo* InView)
		: Scene(InScene),
		  View(InView)
	{
	}

	virtual ~WMeshPassProcessor() = default;

	template<typename PassShaderType>
	void BuildMeshDrawCommand(
	const WMeshBatch MeshBatch,
	const WMeshPassProcessorRenderState& RenderState,
	PassShaderType *PassShader,
	MaterialProxy *Material,
	EPassFeature Feature);

protected:

	const RScene *Scene;

	const WSceneViewInfo *View;

};

template<typename VertexType, typename GeometryType, typename PixelType>
struct WMeshPassProcessorShader
{
public:

	WMeshPassProcessorShader() = default;

	~WMeshPassProcessorShader() = default;

	WVertexShaderRHIRef GetVertexShaderRHI() const { return VertexShader ? VertexShader->GetVertexShader() : nullptr; }

	WGeometryShaderRHIRef GetGeometryShaderRHI() const { return GeometryShader ? GeometryShader->GetGeometryShader() : nullptr; }

	WPixelShaderRHIRef GetPixelShaderRHI() const { return PixelShader ? PixelShader->GetPixelShader() : nullptr; }

public:

	VertexType *VertexShader = nullptr;

	GeometryType *GeometryShader = nullptr;

	PixelType *PixelShader = nullptr;

};