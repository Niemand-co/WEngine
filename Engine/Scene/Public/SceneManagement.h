#pragma once
#include "Render/Mesh/Public/MeshBatch.h"

class WMeshCollector
{
public:

	WMeshCollector() = default;

	~WMeshCollector() = default;

	WMeshBatch& AllocateMesh()
	{
		Batches.Push(WMeshBatch());
		return *(Batches.end() - 1);
	}

	const WEngine::WArray<WMeshBatch>& GetBatches() const { return Batches; }

private:

	WEngine::WArray<WMeshBatch> Batches;

};