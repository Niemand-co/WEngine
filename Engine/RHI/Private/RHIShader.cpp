#include "pch.h"
#include "RHI/Public/RHIShader.h"

const char* RHIShader::GetEntry()
{
	return m_entryName.Data();
}
