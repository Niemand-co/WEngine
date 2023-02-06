#pragma once
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Render/Mesh/Public/Mesh.h"

class StaticMeshComponent : public PrimitiveComponent
{
public:

	friend class StaticMeshInfo;
	
	enum { type = 6 };

	StaticMeshComponent(GameObject *pGameObject, StaticMesh *pMesh = nullptr);

	virtual ~StaticMeshComponent();

	virtual PrimitiveProxy* GetPrimitiveProxy() override;

	StaticMesh* GetStaticMesh() const { return m_pMesh; }

	uint8_t IsCastShadow() const { return m_bCastShadow; }

protected:

	StaticMesh *m_pMesh;

	uint8_t m_bCastShadow : 1;

};

struct StaticMeshProxy : public PrimitiveProxy
{
	StaticMeshProxy(StaticMeshComponent *primitive)
		: PrimitiveProxy(primitive)
	{

	}

	virtual void DrawStaticMesh(RHICommandListBase* CmdList) override;

	enum { type = 1 };

};