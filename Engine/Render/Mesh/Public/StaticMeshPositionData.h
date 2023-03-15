#pragma once
#include "Render/Mesh/Public/StaticMeshVertexDataInterface.h"

class WStaticMeshPositionData : public WStaticMeshVertexDataInterface
{
public:

	WStaticMeshPositionData() = default;

	virtual ~WStaticMeshPositionData() = default;

	virtual void ResizeBuffer(uint32 NumVertices) override { Data.Reserve(NumVertices); }

	virtual void Empty() override { Data.Reserve(0); }

	virtual uint32 GetNum() const override { return Data.Size(); }

	virtual uint8* GetData() const override { return (uint8*)Data.GetData(); }

	virtual uint32 GetStride() const override { return sizeof(glm::vec3); }

	virtual size_t GetSize() const override { return Data.Size() * sizeof(glm::vec3); }

private:

	WEngine::WArray<glm::vec3> Data;

};