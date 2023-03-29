#pragma once
#include "Render/Mesh/Public/StaticMeshVertexDataInterface.h"

class WStaticMeshIndexData : public WStaticMeshVertexDataInterface
{
public:

	WStaticMeshIndexData() = default;

	virtual ~WStaticMeshIndexData() = default;

	virtual void ResizeBuffer(uint32 NumIndices) override { Data.Reserve(NumIndices); }

	virtual void Empty() override { Data.Reserve(0); }

	virtual uint32 GetNum() const override { return Data.Size(); }

	virtual uint8* GetData() const override { return (uint8*)Data.GetData(); }

	virtual uint32 GetStride() const override { return 4; }

	virtual size_t GetSize() const override { return Data.Size() * 4; }

private:

	WEngine::WArray<uint32> Data;

};