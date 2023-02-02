#pragma once

class WBuffer : public RenderResource
{
public:

	virtual ~WBuffer() = default;

protected:

	WEngine::WSharedPtr<class RHIBuffer> m_pBuffer;

};

class WVertexBuffer : public RenderResource
{
public:

	WVertexBuffer();

	virtual ~WVertexBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};

class WIndexBuffer : public WBuffer
{
public:

	WIndexBuffer();

	virtual ~WIndexBuffer();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

};

class WUniformBuffer : public WBuffer
{
public:

	WUniformBuffer();

	virtual ~WUniformBuffer();



};