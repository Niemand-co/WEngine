#pragma once
#include "Render/Public/RenderResource.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/NamingSystem.h"

class WShader : public RenderResource
{
public:

	WShader() = default;

	virtual ~WShader() = default;

protected:

	ShaderCodeBlob Blob;

};

class WMaterialShader : public WShader
{
public:

	WMaterialShader() = default;

	virtual ~WMaterialShader() = default;

	virtual void GetParametersBinding(class RScene *Scene, class MaterialProxy *Material) = 0;

};

class WComputeShader : public WShader
{
public:

	WComputeShader(ShaderCodeBlob& inBlob)
		: WShader(inBlob)
	{
	}

	void InitRHIResource() override;

	void ReleaseRHIResource() override;

	void UpdateRHIResource() override;

	WComputeShaderRHIRef GetRHI() const { return Shader; }

private:

	WComputeShaderRHIRef Shader;

};

size_t ShaderHash(WEngine::WGuid<WEngine::WString> key);

class WShaderLibrary : public WEngine::NamingSystem
{
public:

	static WShader* GetShader(const WEngine::WString& name)
	{
		return Shaders[WEngine::WGuid(name)];
	}

	static bool LoadShader(const WEngine::WString& path);

private:

	static WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WShader*, ShaderHash> Shaders;

};