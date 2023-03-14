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

	EShadingPath GetShadingPath() const { return ShadingPath; }

	void AddPrimitive(class PrimitiveComponent *primitive);

	void RemovePrimitive(PrimitiveComponent *primitive);

	void UpdatePrimitiveInfosForScene();

	void AddLight(class LightComponent *light);

	void RemoveLight(LightComponent *light);

	void UpdateLightInfosForScene();

	void AddCamera(class CameraComponent *camera);

	void RemoveCamera(CameraComponent *camera);

	void UpdateCameraInfoForScene();

	const WEngine::WArray<PrimitiveInfo*>& GetPrimitives() const { return m_primitives; }

	const WEngine::WArray<uint8>& GetPrimitiveMasks() const { return m_primitiveMasks; }

	const WEngine::WArray<PrimitiveInfo*>& GetDynamicShadowCaster() const { return m_dynamicShadowCaster; }

	const WEngine::WArray<LightInfo*>& GetLights() const { return m_lights; }

	const WEngine::WArray<CameraInfo*>& GetCameras() const { return m_cameras; }

	LightInfo* GetMainLightInfo() const { return m_pMainLight; }

	void* operator new(size_t size)
	{
		return NormalAllocator::Get()->Allocate(size);
	}

	void operator delete(void* pData)
	{
		NormalAllocator::Get()->Deallocate(pData);
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

	WEngine::WArray<uint8> m_primitiveMasks;

	WEngine::WArray<PrimitiveInfo*> m_dynamicShadowCaster;

	WEngine::WArray<CameraInfo*> m_cameras;

	WEngine::WSet<CameraInfo*> m_addedCameras;

	WEngine::WSet<CameraInfo*> m_removedCameras;

	EShadingPath ShadingPath;

};