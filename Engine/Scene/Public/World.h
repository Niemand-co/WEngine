#pragma once

class GameObject;
class Camera;

class World
{
public:
	World();

	~World();

	GameObject* CreateGameObject(std::string name = "GameObject");

	void AddCamera(Camera *pCamera);

	std::vector<Camera*>& GetCameras();

	static World* CreateWorld();

	static World* GetWorld();

	const std::vector<GameObject*>& GetGameObjects() const;

	const std::vector<char*>& GetObjectNames() const;

private:

	std::vector<GameObject*> m_pEntities;

	std::vector<char*> m_pNames;

	std::vector<Camera*> m_pCameras;

	static World* g_pInstance;

};