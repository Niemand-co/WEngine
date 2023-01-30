#pragma once
#include "Scene/Components/Public/StaticMeshComponent.h"

class InstancedStaticMeshComponent : public StaticMeshComponent
{
public:

	enum { type = 7 };

	InstancedStaticMeshComponent(GameObject *pGameObject);

	virtual ~InstancedStaticMeshComponent();

	virtual PrimitiveInfo* GetPrimitiveInfo() override;

	unsigned int GetInstanceCount() const { return m_instancedCount; }

	TransformComponent* GetTransform(unsigned instanceIndex) const { RE_ASSERT(instanceIndex < m_instancedCount, "Out of Instance Index."); return m_transforms[instanceIndex]; }

	const WEngine::WArray<TransformComponent*>& GetTransforms() const { return m_transforms; }

	void AddInstance();

private:

	unsigned int m_instancedCount;

	WEngine::WArray<TransformComponent*> m_transforms;

};

struct InstancedStaticMeshInfo : public StaticMeshInfo
{
	InstancedStaticMeshInfo(InstancedStaticMeshComponent* primitive)
		: StaticMeshInfo(primitive),
		  InstanceCount(primitive->GetInstanceCount())
	{
		LocalToWorldMatrices.Reserve(primitive->GetInstanceCount());
		for (TransformComponent* transform : primitive->GetTransforms())
		{
			LocalToWorldMatrices.Push(transform->GetLocalToWorldMatrix());
		}
	}

	enum { type = 2 };

	unsigned int InstanceCount;

	WEngine::WArray<glm::mat4> LocalToWorldMatrices;
};