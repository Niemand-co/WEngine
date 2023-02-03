#pragma once

class WMeshBatchElement
{
public:

	WMeshBatchElement();

	~WMeshBatchElement();

};

class WMeshBatch
{
public:

	WMeshBatch();

	~WMeshBatch();

private:

	WEngine::WArray<WMeshBatchElement*> Elements;

};