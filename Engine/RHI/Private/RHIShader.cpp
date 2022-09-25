#include "pch.h"
#include "RHI/Public/RHIShader.h"

ShaderStage RHIShader::GetStage()
{
	return m_stage;
}

const char* RHIShader::GetEntry()
{
	return m_entryName.c_str();
}
