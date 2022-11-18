#include "pch.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"

World* World::g_pInstance = nullptr;

World::World()
{
	m_pMainLight = nullptr;
}

World::~World()
{

}

GameObject* World::CreateGameObject(std::string name)
{
	GameObject* gameObject = new GameObject(name);
	g_pInstance->m_pEntities.push_back(std::move(gameObject));
	g_pInstance->m_pNames.push_back(name.data());
	gameObject->AddComponent<Transformer>();
	return gameObject;
}

void World::AddCamera(Camera* pCamera)
{
	m_pCameras.push_back(std::move(pCamera));
}

void World::AddLight(Light* pLight)
{
	if (m_pMainLight == nullptr)
	{
		m_pMainLight = pLight;
		pLight->SetMainLight(true);
	}
	m_pAdditionalLights.push_back(pLight);
}

void World::SetMainLight(Light* pLight)
{
	for (unsigned int i = 0; i < m_pAdditionalLights.size(); ++i)
	{
		if (m_pAdditionalLights[i] == pLight)
		{
			m_pMainLight->SetMainLight(false);
			pLight->SetMainLight(true);
			m_pAdditionalLights[i] = m_pMainLight;
			m_pMainLight = pLight;
			return;
		}
	}
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

const std::vector<GameObject*>& World::GetGameObjects() const
{
	return m_pEntities;
}

const std::vector<char*>& World::GetObjectNames() const
{
	return m_pNames;
}

std::vector<Camera*>& World::GetCameras()
{
	return m_pCameras;
}

Light* World::GetMainLight() const
{
	return m_pMainLight;
}

const std::vector<Light*>& World::GetAdditionalLights() const
{
	return m_pAdditionalLights;
}
