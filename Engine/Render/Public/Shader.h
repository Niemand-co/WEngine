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

	virtual void GetParametersBinding(class RScene *Scene, class MaterialProxy *Material) = 0;

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

size_t ShaderHash(WEngine::WGuid<WEngine::WString> key);

class WShaderLibrary : public WEngine::NamingSystem
{
public:

	static class RHIShader* GetShader(const WEngine::WString& name)
	{
		return Shaders[WEngine::WGuid(name)];
	}

	static bool LoadShader(const WEngine::WString& path);

private:

	static WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, RHIShader*, ShaderHash> Shaders;

};