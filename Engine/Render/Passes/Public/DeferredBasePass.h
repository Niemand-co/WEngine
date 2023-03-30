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

	virtual void GetParametersBinding(const WViewInfo* View, const MaterialProxy* Material) override;

	virtual WVertexShaderRHIRef GetVertexShader() const override { return VertexShaderRHI; }

	BEGIN_SHADER_PARAMETERS_STRUCT(WDeferredBasePassVSParameters)
	END_SHADER_PARAMETERS_STRUCT

private:

private:

	WVertexShaderRHIRef VertexShaderRHI;

};

class WDeferredBasePassPS : public WMaterialShader
{
public:

	WDeferredBasePassPS();

	virtual ~WDeferredBasePassPS();

	virtual void GetParametersBinding(const WViewInfo* View, const MaterialProxy* Material) override;

	virtual WPixelShaderRHIRef GetPixelShader() const override { return PixelShaderRHI; }

private:

	WPixelShaderRHIRef PixelShaderRHI;

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

void GetBasePassShaders(WDeferredBasePassVS*& VertexShader, WDeferredBasePassPS*& PixelShader);