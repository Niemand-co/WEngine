#pragma once
#include "Render/Public/RenderResource.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/NamingSystem.h"

class WShader : public RenderResource
{
public:

	WShader() = default;

	virtual ~WShader() = default;

	virtual WShaderRHIRef GetVertexShader() const { return nullptr; }

	virtual WShaderRHIRef GetGeometryShader() const { return nullptr; }

	virtual WShaderRHIRef GetPixelShader() const { return nullptr; }

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

	WShaderRHIRef GetRHI() const { return Shader; }

private:

	WShaderRHIRef Shader;

};

size_t ShaderHash(WEngine::WString Name);

class WShaderLibrary
{
public:

	static WShaderRHIRef GetShader(const WEngine::WString& Name)
	{
		if (Shaders.Find(Name))
		{
			return Shaders[Name];
		}
		return nullptr;
	}

	static void AddShader(const WEngine::WString& Name, WShaderRHIRef Shader)
	{
		Shaders[Name] = Shader;
	}

	static bool LoadShader(const WEngine::WString& path);

private:

	static WEngine::WHashMap<WEngine::WString, WShaderRHIRef, ShaderHash> Shaders;

};