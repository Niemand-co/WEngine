#pragma once

class GameObject;
class CameraComponent;
class LightComponent;

class World
{
public:
	World();

	~World();

	GameObject* CreateGameObject(const WEngine::WString& name = "GameObject");

	void AddCamera(CameraComponent*pCamera);

	void AddLight(LightComponent*pLight);

	void SetMainLight(LightComponent*pLight);

	WEngine::WArray<CameraComponent*>& GetCameras();

	LightComponent* GetMainLight() const;

	const WEngine::WArray<LightComponent*>& GetAdditionalLights() const;

	static World* CreateWorld();

	static World* GetWorld();

	const WEngine::WArray<GameObject*>& GetGameObjects() const;

	const WEngine::WArray<WEngine::WString>& GetObjectNames() const;

private:

	WEngine::WArray<GameObject*> m_pEntities;

	WEngine::WArray<WEngine::WString> m_pNames;

	WEngine::WArray<CameraComponent*> m_pCameras;

	WEngine::WArray<LightComponent*> m_pAdditionalLights;

	CameraComponent*m_pEditorCamera;

	LightComponent *m_pMainLight;

	static World* g_pInstance;

};