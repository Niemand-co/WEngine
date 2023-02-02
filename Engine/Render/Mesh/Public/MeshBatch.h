#pragma once

class WMeshBatchElement
{
public:

	

};

class WMeshBatch
{
public:

	WMeshBatch();

	~WMeshBatch();

private:

	WEngine::WArray<WMeshBatchElement*> Elements;

};