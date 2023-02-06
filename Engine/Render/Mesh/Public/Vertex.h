#pragma once

struct VertexBindingDescription
{
	unsigned int slot;
	size_t stride;
};

struct VertexAttributeDescription
{
	unsigned int slot;
	unsigned int location;
	unsigned int offset;
	Format format;
};

class RHIVertexInputDescriptor;

class WVertexFactory : public RenderResource
{
public:

	WVertexFactory();

	virtual ~WVertexFactory();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

	static void GenerateVertexInputDescription();

	static RHIVertexInputDescriptor GetVertexInputDescriptor();

public:

	struct VertexStream
	{
		const WEngine::WSharedPtr<class WVertexBuffer> m_pVertexBuffer = nullptr;
		uint32 Offset = 0;
		uint16 Stride = 0;
	};

private:

	WEngine::WArray<VertexStream> m_streams;

	static VertexBindingDescription* m_bindingDescription;

	static WEngine::WArray<VertexAttributeDescription*> m_attributeDescriptions;
	
};

class WLocalVertexFactory : public WVertexFactory
{
public:

	WLocalVertexFactory();

	virtual ~WLocalVertexFactory();



};

class WNormalVertexFactory : public WVertexFactory
{
public:

	WNormalVertexFactory();

	virtual ~WNormalVertexFactory();

};