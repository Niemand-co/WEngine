#pragma once

struct PrimitiveInfo;
struct LightInfo;
struct CameraInfo;

class RScene
{
public:

	friend struct PrimitiveInfo;

	RScene();

	~RScene();

	void AddPrimitive(class PrimitiveComponent *primitive);

	void RemovePrimitive(PrimitiveComponent *primitive);

	void UpdatePrimitiveInfosForScene();

	void AddLight(class LightComponent *light);

	void RemoveLight(LightComponent *light);

	void UpdateLightInfosForScene();

	void AddCamera(class CameraComponent *camera);

	void RemoveCamera(CameraComponent *camera);

	void UpdateCameraInfoForScene();

	void StartFrame(class WViewport *viewport);

	void StartRendering(class WViewport *viewport);

	WEngine::WArray<PrimitiveInfo*> GetOpaqueAndMaskPrimitives() const { return m_opaqueAndMaskPrimitives; }

	WEngine::WArray<PrimitiveInfo*> GetTranslucentPrimitives() const { return m_translucentPrimitives; }

	WEngine::WArray<PrimitiveInfo*> GetDynamicShadowCaster() const { return m_dynamicShadowCaster; }

	WEngine::WArray<LightInfo*> GetLightInfos() const { return m_lights; }

	LightInfo* GetMainLightInfo() const { return m_pMainLight; }

	void* operator new(size_t size)
	{
		return WEngine::Allocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		WEngine::Allocator::Get()->Deallocate(pData);
	}

public:

	static void SetActiveScene(RScene *scene) { g_activeScene = scene; }

	static RScene* GetActiveScene() { return g_activeScene; }

private:

	static RScene *g_activeScene;

private:

	class GWorld* m_pWorld;

	WEngine::WArray<PrimitiveInfo*> m_primitives;

	WEngine::WArray<glm::mat4> m_primitiveMatrices;

	WEngine::WSet<PrimitiveInfo*> m_addedPrimitives;

	WEngine::WSet<PrimitiveInfo*> m_removedPrimitives;

	WEngine::WArray<LightInfo*> m_lights;

	LightInfo *m_pMainLight;

	WEngine::WSet<LightInfo*> m_addedLights;

	WEngine::WSet<LightInfo*> m_removedLights;

	WEngine::WArray<PrimitiveInfo*> m_opaqueAndMaskPrimitives;

	WEngine::WArray<PrimitiveInfo*> m_translucentPrimitives;

	WEngine::WArray<PrimitiveInfo*> m_dynamicShadowCaster;

	WEngine::WArray<CameraInfo*> m_cameras;

	WEngine::WSet<CameraInfo*> m_addedCameras;

	WEngine::WSet<CameraInfo*> m_removedCameras;

};