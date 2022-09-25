#pragma once
#ifndef SR_WORLD_H
#define SR_WORLD_H

class GameObject;

class World
{
public:
	World();
	~World();

	static World* CreateWorld();

	template<typename T = GameObject>
	static T* CreateGameObject(World* world);

private:
	std::vector<GameObject*> m_entities;
	static World* m_instance;
};

template<typename T>
T* World::CreateGameObject(World* world)
{
	T* entity = new T();
	world->m_entities.push_back(entity); 
	return entity;
}

#endif