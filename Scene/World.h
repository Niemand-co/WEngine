#pragma once
#ifndef SR_WORLD_H
#define SR_WORLD_H

#include "Entity.h"
#include "Utils/Window.h"
#include "Render/Renderer.h"
#include <vector>
#include <unordered_map>

class World
{
public:
	World();
	~World();

	static World* CreateWorld();

	template<typename T = Entity>
	static T* CreateEntity(World* world);

	void Render();

	void SetRenderer(Entity* entity, Renderer* renderer);
private:
	std::vector<Entity*> m_entities;
	std::unordered_map<Entity*, Renderer*> m_renderqueque;
	static World* m_instance;
};

template<typename T>
T* World::CreateEntity(World* world)
{
	T* entity = new T();
	world->m_entities.push_back(entity);
	return entity;
}

#endif