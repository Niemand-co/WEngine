#pragma once

class RHIRenderTarget
{
public:

	virtual ~RHIRenderTarget() = default;

	virtual unsigned int GetWidth();

	virtual unsigned int GetHeight();

protected:

	unsigned int m_width;

	unsigned int m_height;

};