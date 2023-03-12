#pragma once
#include "Render/Mesh/Public/StaticMeshVertexDataInterface.h"

template<typename DataType>
class WStaticMeshVertexData : public WStaticMeshVertexDataInterface
{
public:

	WStaticMeshVertexData() = default;

	virtual ~WStaticMeshVertexData() = default;

	virtual void ResizeBuffer(uint32 NumVertices) override;

	virtual void Empty() override;

	virtual uint32 GetNum() const { return Data.Size() }

	virtual uint8* GetData() const { return (uint8*)Data.GetData(); }

	virtual uint32 GetStride() const { return sizeof(DataType); }

	virtual size_t GetSize() const { return sizeof(DataType) * Data.Size(); }

private:

	WEngine::WArray<DataType> Data;

};

template<typename DataType>
inline void WStaticMeshVertexData<DataType>::ResizeBuffer(uint32 NumVertices)
{
	Data.Resize(NumVertices);
}

template<typename DataType>
inline void WStaticMeshVertexData<DataType>::Empty()
{
	Data.Resize(0);
}