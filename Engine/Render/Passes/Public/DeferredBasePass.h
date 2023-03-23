#pragma once
#include "Render/Public/Shader.h"
#include "Render/Public/MeshPassProcessor.h"
#include "Render/Public/MeshPassProcessor.inl"
#include "Render/Public/RenderDependencyGraphParameter.h"

class WDeferredBasePassVS : public WMaterialShader
{
public:

	WDeferredBasePassVS();

	virtual ~WDeferredBasePassVS();

	virtual void GetParametersBinding(class RScene* Scene, class MaterialProxy* Material) override;

	WVertexShaderRHIRef GetVertexShader() const { return VertexShaderRHI; }

private:

	WVertexShaderRHIRef VertexShaderRHI;

};

class WDeferredBasePassMeshProcessor : public WMeshPassProcessor
{
public:

	WDeferredBasePassMeshProcessor(const class RScene *InScene, const WViewInfo* InView, const WMeshPassProcessorRenderState& InRenderState);

	virtual ~WDeferredBasePassMeshProcessor() = default;

	void AddMeshBatch(const class WMeshBatch& MeshBatch);

private:

	bool ProcessDeferredShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material);

	bool ProcessForwardShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material);

	bool ProcessForwardPlusShadingPath(const WMeshBatch& MeshBatch, const MaterialProxy& Material);

private:

	const WMeshPassProcessorRenderState RenderState;

};