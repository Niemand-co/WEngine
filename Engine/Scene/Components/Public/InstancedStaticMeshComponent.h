#pragma once
#include "Scene/Components/Public/StaticMeshComponent.h"

class InstancedStaticMeshComponent : public StaticMeshComponent
{
public:

	enum { type = 7 };

	InstancedStaticMeshComponent(GameObject *pGameObject);

	virtual ~InstancedStaticMeshComponent();

	virtual PrimitiveProxy* GetPrimitiveProxy() override;

	unsigned int GetInstanceCount() const { return m_instancedCount; }

	TransformComponent* GetTransform(unsigned instanceIndex) const { RE_ASSERT(instanceIndex < m_instancedCount, "Out of Instance Index."); return m_transforms[instanceIndex]; }

	const WEngine::WArray<TransformComponent*>& GetTransforms() const { return m_transforms; }

	void AddInstance();

private:

	unsigned int m_instancedCount;

	WEngine::WArray<TransformComponent*> m_transforms;

};

struct InstancedStaticMeshProxy : public StaticMeshProxy
{
	InstancedStaticMeshProxy(InstancedStaticMeshComponent* primitive)
		: StaticMeshProxy(primitive)
	{

	}

	enum { type = 2 };

	unsigned int InstanceCount;

	WEngine::WArray<glm::mat4> LocalToWorldMatrices;
};