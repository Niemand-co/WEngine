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

const std::vector<GameObject*>& GameObject::GetChildGameObjects() const
{
	return m_sonGameObjects;
}

void GameObject::ShowInInspector() const
{
	for (Component* pComponent : m_components)
	{
		pComponent->ShowInInspector();
	}
}

bool GameObject::HasChild() const
{
	return m_sonGameObjects.size() != 0;
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
