#include "pch.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"

GWorld* GWorld::g_pInstance = nullptr;

GWorld::GWorld()
{
	m_pMainLight = nullptr;
}

GWorld::~GWorld()
{

}

GameObject* GWorld::CreateGameObject(const WEngine::WString& name)
{
	GameObject* gameObject = new GameObject(name);
	g_pInstance->m_pEntities.Push(std::move(gameObject));
	g_pInstance->m_pNames.Push(name);
	gameObject->AddComponent<TransformComponent>();
	return gameObject;
}

void GWorld::AddCamera(CameraComponent* pCamera)
{
	m_pCameras.Push(std::move(pCamera));
}

void GWorld::AddLight(LightComponent* pLight)
{
	if (m_pMainLight == nullptr)
	{
		m_pMainLight = pLight;
		pLight->SetMainLight(true);
	}
	m_pAdditionalLights.Push(pLight);
}

void GWorld::SetMainLight(LightComponent* pLight)
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

GWorld* GWorld::CreateWorld()
{
	if (g_pInstance == nullptr)
	{
		g_pInstance = (GWorld*)WEngine::Allocator::Get()->Allocate(sizeof(GWorld));
		::new (g_pInstance) GWorld();
		return g_pInstance;
	}
	else
	{
		std::cout<<"Over Created World"<<std::endl;
		exit(0);
	}
}

GWorld* GWorld::GetWorld()
{
	return g_pInstance;
}

const WEngine::WArray<GameObject*>& GWorld::GetGameObjects() const
{
	return m_pEntities;
}

const WEngine::WArray<WEngine::WString>& GWorld::GetObjectNames() const
{
	return m_pNames;
}

WEngine::WArray<CameraComponent*>& GWorld::GetCameras()
{
	return m_pCameras;
}

LightComponent* GWorld::GetMainLight() const
{
	return m_pMainLight;
}

const WEngine::WArray<LightComponent*>& GWorld::GetAdditionalLights() const
{
	return m_pAdditionalLights;
}
