#pragma once

class WMeshBatchElement
{
public:

	WMeshBatchElement() = default;

	~WMeshBatchElement() = default;

public:

	WEngine::WSharedPtr<class WIndexBuffer> IndexBuffer;

	WEngine::WSharedPtr<class WUniformBuffer> UniformBuffer;

};

class WMeshBatch
{
public:

	WMeshBatch() = default;

	~WMeshBatch() = default;

public:

	WEngine::WArray<WMeshBatchElement> Elements = WEngine::WArray<WMeshBatchElement>(1);

	WEngine::WSharedPtr<class WVertexFactory> VertexFactory;

	WEngine::WSharedPtr<class MaterialProxy> Material;

};