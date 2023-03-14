#pragma once

class WMeshBatchElement
{
public:

	WMeshBatchElement() = default;

	~WMeshBatchElement() = default;

public:

	class WIndexBuffer *IndexBuffer;

	class WUniformBuffer *UniformBuffer;

};

class WMeshBatch
{
public:

	WMeshBatch() = default;

	~WMeshBatch() = default;

public:

	WEngine::WArray<WMeshBatchElement> Elements = WEngine::WArray<WMeshBatchElement>(1);

	class WVertexFactory* VertexFactory;

	class MaterialProxy* Material;

};