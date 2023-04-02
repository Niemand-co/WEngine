#pragma once
#include "Render/Public/RenderResource.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/NamingSystem.h"

class ShaderMappingInfo
{
public:

	ShaderMappingInfo(WMaterialShader *Shader);

	~ShaderMappingInfo();

private:

	

};

class WShader : public RenderResource
{
public:

	WShader() = default;

	virtual ~WShader() = default;

	virtual WVertexShaderRHIRef GetVertexShader() const { return nullptr; }

	virtual WGeometryShaderRHIRef GetGeometryShader() const { return nullptr; }

	virtual WPixelShaderRHIRef GetPixelShader() const { return nullptr; }

};

class WMaterialShader : public WShader
{
public:

	typedef WMaterialShader ShaderType;

	WMaterialShader() = default;

	virtual ~WMaterialShader() = default;

	void InitRHIResource() override {}

	void ReleaseRHIResource() override {}

	void UpdateRHIResource() override {}

	virtual void GetParametersBinding(const class WViewInfo* View, const struct MaterialProxy* Material, struct WMeshDrawShaderBindings& Bindings) {}

};

class WDummyMaterialShader : public WMaterialShader
{
};

class WComputeShader : public WShader
{
public:

	WComputeShader()
	{
	}

	void InitRHIResource() override;

	void ReleaseRHIResource() override;

	void UpdateRHIResource() override;

	WComputeShaderRHIRef GetRHI() const { return Shader; }

private:

	WComputeShaderRHIRef Shader;

};