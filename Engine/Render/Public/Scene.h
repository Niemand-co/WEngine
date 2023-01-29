#pragma once

struct PrimitiveInfo;
struct LightInfo;
struct CameraInfo;

class RScene
{
public:

	RScene();

	~RScene();

	void AddPrimitive(class PrimitiveComponent *primitive);

	void RemovePrimitive(PrimitiveComponent *primitive);

	void UpdatePrimitiveInfosForScene();

	void AddLight(class LightComponent *light);

	void RemoveLight(LightComponent *light);

	void UpdateLightInfosForScene();

	void UpdateCameraInfosForScene();

public:

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

	WEngine::WSet<LightInfo*> m_addedLights;

	WEngine::WSet<LightInfo*> m_removedLights;

	WEngine::WArray<CameraInfo*> m_cameras;

	WEngine::WSet<CameraInfo*> m_addedCameras;

	WEngine::WSet<CameraInfo*> m_removedCameras;

};