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

	virtual void GetParametersBinding(class RScene* Scene, MaterialProxy* Material) override;

	WVertexShaderRHIRef GetVertexShader() const { return VertexShaderRHI; }

private:

	WVertexShaderRHIRef VertexShaderRHI;

};

class WDeferredBasePassMeshProcessor : public WMeshPassProcessor
{
public:

	WDeferredBasePassMeshProcessor(const class RScene *InScene, const WViewInfo* InView, const WMeshPassProcessorRenderState& InRenderState);

	virtual ~WDeferredBasePassMeshProcessor() = default;

private:

	const WMeshPassProcessorRenderState RenderState;

};