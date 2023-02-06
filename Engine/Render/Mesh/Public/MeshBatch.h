#pragma once

class WMeshBatchElement
{
public:

	WMeshBatchElement();

	~WMeshBatchElement();

private:

	WEngine::WSharedPtr<class WIndexBuffer> IndexBuffer;

	WEngine::WSharedPtr<class WUniformBuffer> UniformBuffer;

};

class WMeshBatch
{
public:

	WMeshBatch();

	~WMeshBatch();

private:

	WEngine::WArray<WMeshBatchElement*> Elements;

	WEngine::WSharedPtr<class WVertexFactory> VertexFactory;

	WEngine::WSharedPtr<class MaterialProxy> Material;

};