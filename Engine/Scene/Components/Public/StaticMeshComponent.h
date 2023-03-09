#pragma once
#include "Scene/Components/Public/PrimitiveComponent.h"
#include "Render/Mesh/Public/Mesh.h"

class StaticMeshComponent : public PrimitiveComponent
{
public:

	friend class StaticMeshProxy;
	
	enum { type = 6 };

	StaticMeshComponent(GameObject *pGameObject, WStaticMesh *pMesh = nullptr);

	virtual ~StaticMeshComponent();

	virtual PrimitiveProxy* GetPrimitiveProxy() override;

	WStaticMesh* GetStaticMesh() const { return m_pMesh; }

	uint8_t IsCastShadow() const { return m_bCastShadow; }

protected:

	WStaticMesh*m_pMesh;

	uint8_t m_bCastShadow : 1;

};

struct StaticMeshProxy : public PrimitiveProxy
{
	StaticMeshProxy(StaticMeshComponent *primitive)
		: PrimitiveProxy(primitive),
		  Mesh(primitive->m_pMesh)
	{

	}

	virtual void DrawStaticMesh(RHICommandListBase* CmdList) override;

	virtual void DrawDynamicMesh(RHICommandListBase* CmdList) override {}

	virtual void GetDynamicMeshElements(WEngine::WArray<class SceneViewInfo>& Views, class WMeshCollector& Collector) override;

	virtual void GenerateBoundingBox() override;

	virtual const BoundingBox& GetBoundingBox() override { return Mesh->GetBoundingBox(); }

	enum { type = 1 };

	WStaticMesh*Mesh;

};