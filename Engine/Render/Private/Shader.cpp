#include "pch.h"
#include "Render/Public/Shader.h"
#include "Utils/Public/ShaderCodeBlob.h"
#include "RHI/Public/RHIShader.h"

void WComputeShader::InitRHIResource()
{
}

void WComputeShader::ReleaseRHIResource()
{
}

void WComputeShader::UpdateRHIResource()
{
}

size_t ShaderHash(WEngine::WGuid<WEngine::WString> key)
{
	return (size_t(key.A) << 32) | (size_t(key.B));
}

WEngine::WHashMap<WEngine::WString, RHIShader*, ShaderHash> WShaderLibrary::Shaders = WEngine::WHashMap<WEngine::WString, RHIShader*, ShaderHash>();

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
		RHIShader*shader = GetRenderCommandList()->CreateVertexShader(Blob);
		Shaders.Insert(ShaderName, shader);
		return true;
	}
	else if (ShaderType == "ps")
	{
		ShaderCodeBlob Blob(path);
		RHIShader*shader = GetRenderCommandList()->CreatePixelShader(Blob);
		Shaders.Insert(ShaderName, shader);
		return true;
	}
	else if (ShaderType == "gs")
	{
		ShaderCodeBlob Blob(path);
		RHIShader* shader = GetRenderCommandList()->CreateGeometryShader(Blob);
		Shaders.Insert(ShaderName, shader);
		return true;
	}
	else if (ShaderType == "cs")
	{
		ShaderCodeBlob Blob(path);
		RHIShader* shader = GetRenderCommandList()->CreateComputeShader(Blob);
		Shaders.Insert(ShaderName, shader);
		return true;
	}
	return false;
}

size_t ShaderHash(WEngine::WString Name)
{
	return WEngine::MemCrc32(Name.Data(), Name.Size());
}
