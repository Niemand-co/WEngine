#pragma once
#include "Render/Public/Buffer.h"
#include "Render/Mesh/Public/StaticMeshVertexDataInterface.h"

class WStaticMeshVertexBuffer : public RenderResource
{
public:

	WStaticMeshVertexBuffer();

	virtual ~WStaticMeshVertexBuffer();

	void Init(const WEngine::WArray<struct VertexComponent>& InVertices);

	virtual void InitRHIResource() override;

	virtual void ReleaseRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	WVertexBufferRHIRef TangentBuffer;

	WStaticMeshVertexDataInterface *TangentData;

	WVertexBufferRHIRef TexCoordsBuffer;

	WStaticMeshVertexDataInterface* TexCoordsData;
};