#pragma once
#include "Render/Public/Buffer.h"

class WStaticMeshIndexBuffer : public WIndexBuffer
{
public:

	WStaticMeshIndexBuffer();

	virtual ~WStaticMeshIndexBuffer();

	void Init(const WEngine::WArray<uint32>& InIndices);

	virtual void InitRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	class WStaticMeshVertexDataInterface *IndexData;

};