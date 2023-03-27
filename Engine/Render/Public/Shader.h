#pragma once
#include "Render/Public/RenderResource.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/NamingSystem.h"

class WShader : public RenderResource
{
public:

	WShader() = default;

	virtual ~WShader() = default;

};

class WMaterialShader : public WShader
{
public:

	WMaterialShader() = default;

	virtual ~WMaterialShader() = default;

	void InitRHIResource() override {}

	void ReleaseRHIResource() override {}

	void UpdateRHIResource() override {}

	virtual void GetParametersBinding(class RScene *Scene, struct MaterialProxy *Material) {}

	virtual WVertexShaderRHIRef GetVertexShader() const { return nullptr; }

	virtual WGeometryShaderRHIRef GetGeometryShader() const { return nullptr; }

	virtual WPixelShaderRHIRef GetPixelShader () const { return nullptr; }

};

class WDummyMaterialShader : public WMaterialShader
{
	virtual void GetParametersBinding(class RScene* Scene, class MaterialProxy* Material) override {}
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

template<typename ShaderType>
class WShaderRefBase
{
public:

	WShaderRefBase();

	~WShaderRefBase();

private:

	ShaderType *Shader;
	
};