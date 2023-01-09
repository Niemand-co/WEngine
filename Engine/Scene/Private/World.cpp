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

GameObject* World::CreateGameObject(const WEngine::WString& name)
{
	GameObject* gameObject = new GameObject(name);
	g_pInstance->m_pEntities.Push(std::move(gameObject));
	g_pInstance->m_pNames.Push(name);
	gameObject->AddComponent<Transformer>();
	return gameObject;
}

void World::AddCamera(CameraComponent* pCamera)
{
	m_pCameras.Push(std::move(pCamera));
}

void World::AddLight(Light* pLight)
{
	if (m_pMainLight == nullptr)
	{
		m_pMainLight = pLight;
		pLight->SetMainLight(true);
	}
	m_pAdditionalLights.Push(pLight);
}

void World::SetMainLight(Light* pLight)
{
	for (unsigned int i = 0; i < m_pAdditionalLights.Size(); ++i)
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

const WEngine::WArray<GameObject*>& World::GetGameObjects() const
{
	return m_pEntities;
}

const WEngine::WArray<WEngine::WString>& World::GetObjectNames() const
{
	return m_pNames;
}

WEngine::WArray<Camera*>& World::GetCameras()
{
	return m_pCameras;
}

Light* World::GetMainLight() const
{
	return m_pMainLight;
}

const WEngine::WArray<Light*>& World::GetAdditionalLights() const
{
	return m_pAdditionalLights;
}
