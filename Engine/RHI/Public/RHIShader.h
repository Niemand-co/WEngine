#pragma once

class RHIShader
{
public:

	virtual ~RHIShader() = default;

	virtual ShaderStage GetStage();

	virtual const char* GetEntry();

protected:

	ShaderStage m_stage;

	std::string m_entryName;

};