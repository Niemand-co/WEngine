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

struct WShaderBinding
{
public:

	WShaderBinding() = default;

	~WShaderBinding() = default;

public:

	uint32 Index = 0xffff;

};

struct WShaderParameterLayout
{
public:

	WShaderParameterLayout() = default;

	~WShaderParameterLayout() = default;

public:

	WEngine::WArray<WShaderBinding> ShaderBindings;

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

	virtual void GetParametersBinding(const class WViewInfo* View, const struct MaterialProxy* Material) {}

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

template<typename ShaderType>
class WShaderRefBase
{
public:

	WShaderRefBase();

	~WShaderRefBase();

private:

	ShaderType *Shader;
	
};