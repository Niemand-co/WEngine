#pragma once

class WMeshPassProcessorRenderState
{
public:

	WMeshPassProcessorRenderState() = default;

	WMeshPassProcessorRenderState(class WUniformBufferRHIRef *ViewUniformBuffer, class WUniformBufferRHIRef *PassUniformBuffer);

	~WMeshPassProcessorRenderState() = default;

	void SetDepthStencilState();

	void SetBlendState();

	void SetRasterizationState();

};

class WMeshPassProcessor
{
public:

	WMeshPassProcessor();

	~WMeshPassProcessor();

};