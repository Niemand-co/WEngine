#pragma once

class RHIShader
{
public:

	virtual ~RHIShader() = default;

protected:

};

class RHIVertexShader : public RHIShader
{
public:

	virtual ~RHIVertexShader() = default;

};

class RHIGeometryShader : public RHIShader
{
public:

	virtual ~RHIGeometryShader() = default;

};

class RHIPixelShader : public RHIShader
{
public:

	virtual ~RHIPixelShader() = default;

};

class RHIComputeShader : public RHIShader
{
public:

	virtual ~RHIComputeShader() = default;

};