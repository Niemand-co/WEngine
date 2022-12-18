#pragma once

class RHIShader
{
public:

	virtual ~RHIShader() = default;

	virtual unsigned int GetStage();

	virtual const char* GetEntry();

protected:

	unsigned int m_stage;

	WEngine::WString m_entryName;

};