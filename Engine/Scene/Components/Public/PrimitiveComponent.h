#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Public/GameObject.h"

struct PrimitiveInfo;

class PrimitiveComponent : public Component
{
public:

	PrimitiveComponent(GameObject *pGameObject);

	virtual ~PrimitiveComponent();

	virtual PrimitiveInfo* GetPrimitiveInfo() = 0;

	void MarkDirty() { m_bMarkedDirty = true; }

protected:

	uint8_t m_bMarkedDirty : 1;

	PrimitiveInfo *m_pInfo;

};

struct PrimitiveInfo
{
	PrimitiveInfo(PrimitiveComponent* primitive)
		: LocalToWorldMatrix(primitive->GetOwner()->GetComponent<TransformComponent>()->GetLocalToWorldMatrix()),
		  Owner(primitive->GetOwner())
	{
		
	}

	glm::mat4 LocalToWorldMatrix;

	GameObject *Owner;

};