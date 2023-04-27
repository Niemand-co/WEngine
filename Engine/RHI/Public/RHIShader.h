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

class RHIGraphicsShader : public RHIShader
{
public:

	RHIGraphicsShader(EShaderFrequency InFrequency)
		: RHIShader(InFrequency)
	{
	}

	virtual ~RHIGraphicsShader() = default;

};

class RHIVertexShader : public RHIGraphicsShader
{
public:

	RHIVertexShader()
		: RHIGraphicsShader(EShaderFrequency::SF_Vertex)
	{
	}

	virtual ~RHIVertexShader() = default;

};

class RHIGeometryShader : public RHIGraphicsShader
{
public:

	RHIGeometryShader()
		: RHIGraphicsShader(EShaderFrequency::SF_Geometry)
	{
	}

	virtual ~RHIGeometryShader() = default;

};

class RHIPixelShader : public RHIGraphicsShader
{
public:

	RHIPixelShader()
		: RHIGraphicsShader(EShaderFrequency::SF_Pixel)
	{
	}

	virtual ~RHIPixelShader() = default;

};

class RHIComputeShader : public RHIShader
{
public:

	RHIComputeShader()
		: RHIShader(EShaderFrequency::SF_Compute)
	{
	}

	virtual ~RHIComputeShader() = default;

};