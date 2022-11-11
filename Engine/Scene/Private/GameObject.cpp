#include "pch.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"

GameObject::GameObject(std::string name)
	: m_name(name)
{
	
}

GameObject::~GameObject()
{

}

bool GameObject::IsName(std::string name)
{
	return name == m_name;
}

void GameObject::AddGameObject(GameObject* pGameObject)
{
	m_sonGameObjects.push_back(pGameObject);
}

GameObject* GameObject::Find(std::string name)
{
	for (GameObject* gameObject : World::GetWorld()->GetGameObjects())
	{
		if (gameObject->IsName(name))
		{
			return gameObject;
		}
	}
	RE_LOG("No GameObject Found.");
	return nullptr;
}
