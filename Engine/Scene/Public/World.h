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

	std::vector<Camera*>& GetCameras();

	Light* GetMainLight() const;

	const std::vector<Light*>& GetAdditionalLights() const;

	static World* CreateWorld();

	static World* GetWorld();

	const std::vector<GameObject*>& GetGameObjects() const;

	const std::vector<WEngine::WString>& GetObjectNames() const;

private:

	std::vector<GameObject*> m_pEntities;

	std::vector<WEngine::WString> m_pNames;

	std::vector<Camera*> m_pCameras;

	std::vector<Light*> m_pAdditionalLights;

	Camera *m_pEditorCamera;

	Light *m_pMainLight;

	static World* g_pInstance;

};