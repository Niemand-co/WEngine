#pragma once

class RHIShader
{
public:

	RHIShader(EShaderFrequency InFrequency)
		: ShaderFrequency(InFrequency)
	{
	}

	virtual ~RHIShader() = default;

	template<typename ParameterType>
	void SetupParameters()
	{
		SetupParametersInternal(ParameterType::FTypeInfo::GetStructMetaData()->GetLayout());
	}

	virtual void SetupParametersInternal(const class ShaderParametersLayout& Layout) = 0;

	EShaderFrequency GetFrequency() const { return ShaderFrequency; }

protected:

	EShaderFrequency ShaderFrequency;

};