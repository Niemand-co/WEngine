#include "World.h"

#include <iostream>

World* World::m_instance = nullptr;

World::World()
{
	
}

World::~World()
{

}

World* World::CreateWorld()
{
	if (m_instance == nullptr)
	{
		m_instance = new World();
		return m_instance;
	}
	else
	{
		std::cout<<"Over Created World"<<std::endl;
		exit(0);
	}
}

void World::Render()
{
	for (Entity* entity : m_entities)
	{
		if(!entity->GetVisible())continue;
		for(Light* light : m_lights)
			m_renderqueque[entity]->DrawByIndices(entity, light, m_camera);
	}
}

void World::SetRenderer(Entity* entity, Renderer* renderer)
{
	m_renderqueque[entity] = renderer;
}

void World::AddLight(Light* light)
{
	m_lights.emplace_back(light);
}