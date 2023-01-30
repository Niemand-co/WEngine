#pragma once
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Render/Mesh/Public/Mesh.h"

class StaticMeshComponent : public PrimitiveComponent
{
public:
	
	enum { type = 6 };

	StaticMeshComponent(GameObject *pGameObject);

	virtual ~StaticMeshComponent();

	virtual PrimitiveInfo* GetPrimitiveInfo() override;

	StaticMesh* GetStaticMesh() const { return m_pMesh; }

	uint8_t IsCastShadow() const { return m_bCastShadow; }

protected:

	StaticMesh *m_pMesh;

	uint8_t m_bCastShadow : 1;

};

struct StaticMeshInfo : public PrimitiveInfo
{
	StaticMeshInfo(StaticMeshComponent* primitive)
		: PrimitiveInfo(primitive),
		  Mesh(primitive->GetStaticMesh()),
		  bCastShadow(primitive->IsCastShadow())
	{

	}

	enum { type = 1 };

	StaticMesh *Mesh;

	uint8_t bCastShadow : 1;
};