#pragma once
#include "Render/Public/RenderResource.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "Utils/Public/NamingSystem.h"

class WShader : public RenderResource
{
public:

	WShader(ShaderCodeBlob& inBlob)
		: Blob(inBlob)
	{
	}

	virtual ~WShader() = default;

protected:

	ShaderCodeBlob Blob;

	

};

class WVertexShader : public WShader
{
public:

	WVertexShader(ShaderCodeBlob& inBlob)
		: WShader(inBlob)
	{
	}

	void InitRHIResource() override;

	void ReleaseRHIResource() override;

	void UpdateRHIResource() override;

private:

	WVertexShaderRHIRef Shader;

};

class WPixelShader : public WShader
{
public:

	WPixelShader(ShaderCodeBlob& inBlob)
		: WShader(inBlob)
	{
	}

	void InitRHIResource() override;

	void ReleaseRHIResource() override;

	void UpdateRHIResource() override;

private:

	WPixelShaderRHIRef Shader;

};

class WGeometryShader : public WShader
{
public:

	WGeometryShader(ShaderCodeBlob& inBlob)
		: WShader(inBlob)
	{
	}

	void InitRHIResource() override;

	void ReleaseRHIResource() override;

	void UpdateRHIResource() override;

private:

	WGeometryShaderRHIRef Shader;

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