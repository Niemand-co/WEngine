#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Public/GameObject.h"
#include "Render/Public/Scene.h"

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

	virtual class PrimitiveProxy* GetProxy() = 0;

	static void AddToScene(RHICommandListBase *CmdList, RScene *scene, WEngine::WArray<PrimitiveInfo*>& primitives)
	{
		WEngine::WArray<PrimitiveProxy*> proxies;
		proxies.Reserve(primitives.Size());
		for (PrimitiveInfo* info : primitives)
		{
			proxies.Push(info->GetProxy());
		}
	}

	enum { type = 0 };

	glm::mat4 LocalToWorldMatrix;

	GameObject *Owner;

};

struct PrimitiveProxy
{
	PrimitiveProxy(PrimitiveInfo *info)
	{
		
	}

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

	uint8 bVisible : 1;

	uint8 bStatic : 1;

	uint8 bTranslucent : 1;

	uint8 bCastShadow : 1;

};