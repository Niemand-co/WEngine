#include "pch.h"
#include "RHI/Public/RHIShader.h"

unsigned int RHIShader::GetStage()
{
	return m_stage;
}

const char* RHIShader::GetEntry()
{
	return m_entryName.Data();
}
