#pragma once

class WMeshBatchElement
{
public:

	WMeshBatchElement() = default;

	~WMeshBatchElement() = default;

private:

	WEngine::WSharedPtr<class WIndexBuffer> IndexBuffer;

	WEngine::WSharedPtr<class WUniformBuffer> UniformBuffer;

};

class WMeshBatch
{
public:

	WMeshBatch() = default;

	~WMeshBatch() = default;

private:

	WEngine::WArray<WMeshBatchElement*> Elements;

	WEngine::WSharedPtr<class WVertexFactory> VertexFactory;

	WEngine::WSharedPtr<class MaterialProxy> Material;

};