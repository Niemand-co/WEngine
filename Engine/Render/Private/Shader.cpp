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

size_t ShaderHash(WEngine::WGuid<WEngine::WString> key)
{
	return (size_t(key.A) << 32) | (size_t(key.B));
}

WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WShader*, ShaderHash> WShaderLibrary::Shaders = WEngine::WHashMap<WEngine::WGuid<WEngine::WString>, WShader*, ShaderHash>();

bool WShaderLibrary::LoadShader(const WEngine::WString& path)
{
	int32 start = path.find_last_of('/') + 1;
	int32 end = path.find_last_of('.');
	if(end == -1)return false;
	WEngine::WString ShaderName = path.Substr(start, end - start);
	WEngine::WString ShaderType = path.Substr(end + 1, path.Size() - end - 1);
	if (ShaderType == "vs")
	{
		ShaderCodeBlob Blob(path);
		WShader *shader = new WVertexShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
		return true;
	}
	else if (ShaderType == "ps")
	{
		ShaderCodeBlob Blob(path);
		WShader *shader = new WPixelShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
		return true;
	}
	else if (ShaderType == "gs")
	{
		ShaderCodeBlob Blob(path);
		WShader* shader = new WGeometryShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
		return true;
	}
	else if (ShaderType == "cs")
	{
		ShaderCodeBlob Blob(path);
		WShader* shader = new WComputeShader(Blob);
		Shaders.Insert(ShaderName, shader);
		BeginInitResource(shader);
		return true;
	}
	return false;
}