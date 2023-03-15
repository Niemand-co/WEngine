#pragma once
#include "Render/Descriptor/Public/RHIVertexInputDescriptor.h"
#include "Render/Descriptor/Public/RHIBlendDescriptor.h"
#include "Render/Descriptor/Public/RHIDepthStencilDescriptor.h"
#include "Render/Descriptor/Public/RHIPipelineResourceLayoutDescriptor.h"
#include "Render/Descriptor/Public/RHIRasterizationDescriptor.h"

class WMeshBatch;


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

	void SetDepthStencilState(RHIDepthStencilDescriptor& InDepthStencilState) { DepthStencilState = InDepthStencilState; }

	void SetBlendState(RHIBlendDescriptor& InBlendState) { BlendState = InBlendState; }

	void SetRasterizationState(RHIRasterizationDescriptor& InRasterizationState) { RasterizationState = InRasterizationState; }

	const RHIDepthStencilDescriptor& GetDepthStencilState() const { return DepthStencilState; }

	const RHIBlendDescriptor& GetBlendState() const { return BlendState; }

	const RHIRasterizationDescriptor& GetRasterizationState() const { return RasterizationState; }

private:

	RHIDepthStencilDescriptor DepthStencilState;

	RHIBlendDescriptor BlendState;

	RHIRasterizationDescriptor RasterizationState;

	WUniformBufferRHIRef ViewUniformBuffer;

	WUniformBufferRHIRef PassUniformBuffer;

};

class WMeshPassProcessor
{
public:

	WMeshPassProcessor();

	~WMeshPassProcessor();

	void BuildMeshDrawCommand(
	const WMeshBatch MeshBatch,
	const WMeshPassProcessorRenderState& RenderState,
	const );

};