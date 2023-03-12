#pragma once
#include "Render/Public/Buffer.h"
#include "Render/Mesh/Public/StaticMeshVertexDataInterface.h"

class WPositionVertexBuffer : public WVertexBuffer
{
public:

	WPositionVertexBuffer();

	virtual ~WPositionVertexBuffer();

	void Init(const WEngine::WArray<struct VertexComponent>& InVertices);

	virtual void InitRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	WStaticMeshVertexDataInterface *PositionData;

};