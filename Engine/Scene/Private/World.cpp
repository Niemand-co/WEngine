#include "pch.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Render/Public/RenderCommand.h"

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