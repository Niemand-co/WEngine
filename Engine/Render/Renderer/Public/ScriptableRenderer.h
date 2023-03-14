#pragma once
#include "Render/Public/Scene.h"
#include "Scene/Public/SceneManagement.h"
#include "Render/Public/SceneView.h"

class ScriptableRenderer
{
public:

	ScriptableRenderer() = default;

	virtual ~ScriptableRenderer();

	virtual void Render() = 0;

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
	}

};

struct WViewInfo : public WSceneViewInfo
{
	WEngine::WBitArray PrimitiveVisibilityMap;

	WViewInfo() = default;
	WViewInfo(const WSceneViewInfo& SceneViewInfo);
};

class SceneRenderer : public ScriptableRenderer
{
public:

	SceneRenderer(const class WSceneViewFamily *InViewFamily);

	virtual ~SceneRenderer() = default;

	void ComputeVisibility();

	void FrustumCulling(const WEngine::WArray<class PrimitiveInfo*>& Primitives, WViewInfo& ViewInfo);

	void OcclusionCulling(const WEngine::WArray<class PrimitiveInfo*>& Primitives, WViewInfo& ViewInfo);

	void GatherDynamicMeshElements(WViewInfo& View);

	void SetScene(RScene *inScene) { Scene = inScene; }

public:

	static SceneRenderer* CreateRenderer(const WSceneViewFamily *InViewFamily);

protected:

	RScene *Scene;

	WEngine::WArray<WViewInfo> Views;

	const WSceneViewFamily *ViewFamily;

	WEngine::WArray<WMeshBatch*> Batches;

	WMeshCollector Collector;

	uint8 bUseSphereTest : 1;

	uint8 bUseBoxTest : 1;

};