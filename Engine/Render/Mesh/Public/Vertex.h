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

struct VertexComponent
{
	glm::vec3 Position = glm::vec3();
	glm::vec3 Normal = glm::vec3();
	glm::vec3 Tangent = glm::vec3();
	glm::vec3 BiTangent = glm::vec3();
	glm::vec3 Color = glm::vec3();
	glm::vec2 UVs[8] = {};
};

enum class EInputType : uint8
{
	Default = 0,
	PositionOnly,
	PositionAndNormal,
};

struct WVertexStreamComponent
{
	const class WVertexBuffer* pVertexBuffer = nullptr;
	uint32 Offset = 0;
	uint16 Stride = 0;
	VertexElementType Type = VertexElementType::VET_Float1;
};

struct VertexInputElement
{
	uint32 AttribIndex = 0;
	uint32 Offset = 0;
	uint32 Stride = 0;
	VertexElementType Type = VertexElementType::VET_Float1;
	bool bUseInstance = false;
};

struct WStaticMeshDataType
{
	WVertexStreamComponent PositionComponent;

	WVertexStreamComponent NormalComponent;

	WVertexStreamComponent TangentComponent;

	WVertexStreamComponent ColorComponent;
	
	WEngine::WArray<WVertexStreamComponent> TexCoordsComponent;

	uint32 NumTexCoords = 0;
};

class WVertexFactory : public RenderResource
{
public:

	WVertexFactory();

	virtual ~WVertexFactory();

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;
	
};

class WLocalVertexFactory : public WVertexFactory
{
public:

	WLocalVertexFactory();

	virtual ~WLocalVertexFactory();

	static void GetPSOVertexInputElements(EInputType Type, WEngine::WArray<VertexInputElement>& Elements);

	void SetData(const WStaticMeshDataType& InData) { Data = InData; }

	const WStaticMeshDataType& GetData() const { return Data; }

private:

	WStaticMeshDataType Data;

};

class WNormalVertexFactory : public WVertexFactory
{
public:

	WNormalVertexFactory();

	virtual ~WNormalVertexFactory();

};