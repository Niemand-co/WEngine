#pragma once
#include "Scene/Components/Public/Component.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Public/GameObject.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Public/Scene.h"

struct PrimitiveInfo;
struct PrimitiveProxy;

class PrimitiveComponent : public Component
{
public:

	friend class PrimitiveInfo;

	friend class PrimitiveProxy;

	enum { type = 5 };

	PrimitiveComponent(GameObject *pGameObject);

	virtual ~PrimitiveComponent();

	virtual PrimitiveProxy* GetPrimitiveProxy() = 0;

protected:

	PrimitiveProxy *m_pProxy;

	uint8 m_bVisible : 1;

	uint8 m_bStatic : 1;

	uint8 m_bTranslucent : 1;

	uint8 m_bCastShadow : 1;

};

struct PrimitiveProxy
{
	PrimitiveProxy(PrimitiveComponent* primitive)
		: bVisible(primitive->m_bVisible),
		bStatic(primitive->m_bStatic),
		bTranslucent(primitive->m_bTranslucent),
		bCastShadow(primitive->m_bCastShadow),
		Info(nullptr)
	{
	}

	void SetInfo(PrimitiveInfo* info) { Info = info; }

	PrimitiveInfo* GetInfo() const { return Info; }

	virtual void DrawStaticMesh(RHICommandListBase* CmdList) = 0;

	virtual void DrawDynamicMesh(RHICommandListBase* CmdList) = 0;

	virtual void GetDynamicMeshElements(const WEngine::WArray<class WSceneViewInfo*>& Views, class WMeshCollector& Collector) {}

	virtual void GenerateBoundingBox() = 0;

	virtual const BoundingBox& GetBoundingBox() = 0;

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

	enum { type = 0 };

protected:

	uint8 bVisible : 1;

	uint8 bStatic : 1;

	uint8 bTranslucent : 1;

	uint8 bCastShadow : 1;

	PrimitiveInfo* Info;

};

struct PrimitiveInfo
{
	PrimitiveInfo(PrimitiveComponent* primitive)
		: Owner(primitive->m_pGameObject),
		  Proxy(primitive->m_pProxy),
		  bCastShadow(primitive->m_bCastShadow),
		  bTranslucent(primitive->m_bTranslucent),
		  bStatic(primitive->m_bStatic)
	{
		
	}

	enum class PrimitiveMask : uint8
	{
		None        = 0,
		Dynamic     = 0x01 << 0,
		Translucent = 0x01 << 1,
	};

	PrimitiveProxy* GetProxy() const { return Proxy; }

	static void AddToScene(RHICommandListBase *CmdList, RScene *scene, WEngine::WArray<PrimitiveInfo*>& primitives)
	{
		for (uint32 PrimitiveIndex = 0; PrimitiveIndex < primitives.Size(); ++PrimitiveIndex)
		{
			if (primitives[PrimitiveIndex]->bTranslucent)
			{
				scene->m_primitiveMasks[PrimitiveIndex] |= (uint8)PrimitiveMask::Translucent;
			}

			if (primitives[PrimitiveIndex]->bCastShadow)
			{
				scene->m_primitiveMasks[PrimitiveIndex] |= (uint8)PrimitiveMask::Dynamic;
			}

			if (primitives[PrimitiveIndex]->bStatic)
			{
				primitives[PrimitiveIndex]->Proxy->DrawStaticMesh(CmdList);
			}

			primitives[PrimitiveIndex]->Proxy->GenerateBoundingBox();
		}
	}

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

	GameObject *Owner;

	PrimitiveProxy *Proxy;

	uint8 bCastShadow : 1;

	uint8 bTranslucent : 1;

	uint8 bStatic : 1;

};

ENUM_CLASS_FLAGS(PrimitiveInfo::PrimitiveMask);