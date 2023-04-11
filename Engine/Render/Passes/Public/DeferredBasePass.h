#pragma once
#include "Render/Public/Shader.h"
#include "Render/Public/MeshPassProcessor.h"
#include "Render/Public/MeshPassProcessor.inl"
#include "Render/Public/RenderDependencyGraphParameter.h"
#include "Render/Public/SceneView.h"

class WDeferredBasePassVS : public WMaterialShader
{
public:

	WDeferredBasePassVS(WVertexShaderRHIRef InVertexShaderRHI);

	virtual ~WDeferredBasePassVS();

	virtual void GetParametersBinding(const WViewInfo* View, const MaterialProxy* Material, WMeshDrawShaderBindings& Bindings) override;

	virtual WVertexShaderRHIRef GetVertexShader() const override { return VertexShaderRHI; }

	BEGIN_SHADER_PARAMETERS_STRUCT(WDeferredBasePassVSParameters)
		SHADER_PARAMETER_STRUCT(SceneViewUniformBufferParameters, View)
	END_SHADER_PARAMETERS_STRUCT

private:

	WVertexShaderRHIRef VertexShaderRHI;

	WDeferredBasePassVSParameters Parameters;

	class WUniformBuffer *UniformBuffer;

};

class WDeferredBasePassPS : public WMaterialShader
{
public:

	WDeferredBasePassPS(WPixelShaderRHIRef InPixelShaderRHI);

	virtual ~WDeferredBasePassPS();

	virtual void GetParametersBinding(const WViewInfo* View, const MaterialProxy* Material, WMeshDrawShaderBindings& Bindings) override;

	virtual WPixelShaderRHIRef GetPixelShader() const override { return PixelShaderRHI; }

	BEGIN_SHADER_PARAMETERS_STRUCT(WDeferredBasePassPSParameters)
		SHADER_PARAMETER(glm::vec3, Color)
	END_SHADER_PARAMETERS_STRUCT

private:

	WPixelShaderRHIRef PixelShaderRHI;

	WDeferredBasePassPSParameters Parameters;

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