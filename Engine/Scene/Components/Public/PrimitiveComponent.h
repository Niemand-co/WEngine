#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Public/GameObject.h"

struct PrimitiveInfo;

class PrimitiveComponent : public Component
{
public:

	enum { type = 5 };

	PrimitiveComponent(GameObject *pGameObject);

	virtual ~PrimitiveComponent();

	virtual PrimitiveInfo* GetPrimitiveInfo() = 0;

protected:

	PrimitiveInfo *m_pInfo;

};

struct PrimitiveInfo
{
	PrimitiveInfo(PrimitiveComponent* primitive)
		: LocalToWorldMatrix(primitive->GetOwner()->GetComponent<TransformComponent>()->GetLocalToWorldMatrix()),
		  Owner(primitive->GetOwner())
	{
		
	}

	static void AddToScene(RHICommandListBase *CmdList, class RScene *scene, WEngine::WArray<PrimitiveInfo*>& primitives)
	{
		
	}

	enum { type = 0 };

	glm::mat4 LocalToWorldMatrix;

	GameObject *Owner;

};