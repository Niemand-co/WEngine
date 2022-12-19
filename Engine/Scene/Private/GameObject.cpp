#include "pch.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"

GameObject::GameObject(const WEngine::WString& name)
	: m_name(name), m_id(name)
{
	
}

GameObject::~GameObject()
{

}

bool GameObject::IsName(WEngine::WString name)
{
	return name == m_name;
}

void GameObject::AddGameObject(GameObject* pGameObject)
{
	m_sonGameObjects.Push(pGameObject);
}

const WEngine::WArray<GameObject*>& GameObject::GetChildGameObjects() const
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
	return m_sonGameObjects.Size() != 0;
}

GameObject* GameObject::Find(WEngine::WString name)
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
