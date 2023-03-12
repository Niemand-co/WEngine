#pragma once
#include "Render/Public/Buffer.h"

class WColorVertexBuffer : public WVertexBuffer
{
public:

	WColorVertexBuffer();

	virtual ~WColorVertexBuffer();

	void Init(const WEngine::WArray<struct VertexComponent>& InVertices);

	virtual void InitRHIResource() override;

	virtual void UpdateRHIResource() override;

private:

	class WStaticMeshVertexDataInterface *ColorData;

};