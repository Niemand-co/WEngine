#pragma once

struct WTangentData
{
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

class WStaticMeshVertexDataInterface
{
public:

	virtual ~WStaticMeshVertexDataInterface() = default;

	virtual void ResizeBuffer(uint32 NumVertices) = 0;

	virtual void Empty(uint32 NumVertices) = 0;

	virtual uint32 GetNum() const = 0;

	virtual uint8* GetData() const = 0;

	virtual size_t GetSize() const = 0;

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

};