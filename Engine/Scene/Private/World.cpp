#include "pch.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"

World* World::g_pInstance = nullptr;

World::World()
{
	
}

World::~World()
{

}

GameObject* World::CreateGameObject(std::string name)
{
	GameObject* gameObject = new GameObject(name);
	g_pInstance->m_pEntities.push_back(std::move(gameObject));
	gameObject->AddComponent<Component::ComponentType::Transformer>();
	return gameObject;
}

void World::AddCamera(Camera* pCamera)
{
	m_pCameras.push_back(std::move(pCamera));
}

World* World::CreateWorld()
{
	if (g_pInstance == nullptr)
	{
		g_pInstance = (World*)WEngine::Allocator::Get()->Allocate(sizeof(World));
		::new (g_pInstance) World();
		return g_pInstance;
	}
	else
	{
		std::cout<<"Over Created World"<<std::endl;
		exit(0);
	}
}

World* World::GetWorld()
{
	return g_pInstance;
}

std::vector<GameObject*>& World::GetGameObjects()
{
	return m_pEntities;
}

std::vector<Camera*>& World::GetCameras()
{
	return m_pCameras;
}
