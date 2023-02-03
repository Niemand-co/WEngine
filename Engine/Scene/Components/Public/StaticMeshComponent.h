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
		Proxy = new StaticMeshProxy(this);
	}

	virtual PrimitiveProxy* GetProxy() override
	{
		return Proxy;
	}

	enum { type = 1 };

	StaticMesh *Mesh;

	class StaticMeshProxy *Proxy;

	uint8_t bCastShadow : 1;
};

struct StaticMeshProxy : public PrimitiveProxy
{
	StaticMeshProxy(PrimitiveInfo *info)
		: PrimitiveProxy(info)
	{

	}


};