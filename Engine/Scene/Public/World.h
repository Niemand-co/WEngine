#pragma once

class GameObject;
class Camera;
class Light;

class World
{
public:
	World();

	~World();

	GameObject* CreateGameObject(const WEngine::WString& name = "GameObject");

	void AddCamera(Camera *pCamera);

	void AddLight(Light *pLight);

	void SetMainLight(Light *pLight);

	WEngine::WArray<Camera*>& GetCameras();

	Light* GetMainLight() const;

	const WEngine::WArray<Light*>& GetAdditionalLights() const;

	static World* CreateWorld();

	static World* GetWorld();

	const WEngine::WArray<GameObject*>& GetGameObjects() const;

	const WEngine::WArray<WEngine::WString>& GetObjectNames() const;

private:

	WEngine::WArray<GameObject*> m_pEntities;

	WEngine::WArray<WEngine::WString> m_pNames;

	WEngine::WArray<Camera*> m_pCameras;

	WEngine::WArray<Light*> m_pAdditionalLights;

	Camera *m_pEditorCamera;

	Light *m_pMainLight;

	static World* g_pInstance;

};