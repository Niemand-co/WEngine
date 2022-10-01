#pragma once
#ifndef SR_WORLD_H
#define SR_WORLD_H

class GameObject;
class Camera;

class World
{
public:
	World();

	~World();

	GameObject* CreateGameObject(std::string name = "GameObject");

	void AddCamera(Camera *pCamera);

	static World* CreateWorld();

	static World* GetWorld();

private:

	std::vector<GameObject*> m_pEntities;

	std::vector<Camera*> m_pCameras;

	static World* g_pInstance;

};

#endif