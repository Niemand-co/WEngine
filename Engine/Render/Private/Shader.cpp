#include "pch.h"
#include "Render/Public/Shader.h"
#include "Utils/Public/ShaderCodeBlob.h"

void WVertexShader::InitRHIResource()
{
	Shader = GetRenderCommandList()->CreateVertexShader(Blob);
}

void WVertexShader::ReleaseRHIResource()
{
	Shader = nullptr;
}

void WVertexShader::UpdateRHIResource()
{
}

void WPixelShader::InitRHIResource()
{
	Shader = GetRenderCommandList()->CreatePixelShader(Blob);
}

void WPixelShader::ReleaseRHIResource()
{
	Shader = nullptr;
}

void WPixelShader::UpdateRHIResource()
{
}

void WGeometryShader::InitRHIResource()
{
	Shader = GetRenderCommandList()->CreateGeometryShader(Blob);
}

void WGeometryShader::ReleaseRHIResource()
{
	Shader = nullptr;
}

void WGeometryShader::UpdateRHIResource()
{
}

void WComputeShader::InitRHIResource()
{
	Shader = GetRenderCommandList()->CreateComputeShader(Blob);
}

void WComputeShader::ReleaseRHIResource()
{
	Shader = nullptr;
}

void WComputeShader::UpdateRHIResource()
{
}

size_t Hash(WEngine::WGuid<WEngine::WString> key)
{
	return (size_t(key.A) << 32) | (size_t(key.B));
}

WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WShader*, Hash> WShaderLibrary::Shaders = WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WShader*, Hash>();

void WShaderLibrary::LoadShader(const WEngine::WString& path)
{
	ShaderCodeBlob Blob(path);
	size_t start = path.find_last_of('/') + 1;
	size_t end = path.find_last_of('.');
	WEngine::WString ShaderName = path.Substr(start, end - start);
	WEngine::WString ShaderType = path.Substr(end + 1, 2);
	if (ShaderType == "vs")
	{
		WShader *shader = new WVertexShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
	}
	else if (ShaderType == "ps")
	{
		WShader *shader = new WPixelShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
	}
	else if (ShaderType == "gs")
	{
		WShader* shader = new WGeometryShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
	}
	else if (ShaderType == "cs")
	{
		WShader* shader = new WComputeShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
	}
}