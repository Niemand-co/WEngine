#include "pch.h"
#include "Render/Public/Shader.h"

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