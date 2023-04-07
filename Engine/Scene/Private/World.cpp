#include "pch.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/TransformComponent.h"

GameObject* GWorld::CreateGameObject(const WEngine::WString& name)
{
	GameObject* gameObject = new GameObject(name);
	GetWorld()->m_pEntities.Push(std::move(gameObject));
	GetWorld()->m_pNames.Push(name);
	gameObject->AddComponent<TransformComponent>();
	return gameObject;
}

GWorld* GWorld::GetWorld()
{
	static GWorld *World = new GWorld;
	return World;
}

const WEngine::WArray<GameObject*>& GWorld::GetGameObjects() const
{
	return m_pEntities;
}

const WEngine::WArray<WEngine::WString>& GWorld::GetObjectNames() const
{
	return m_pNames;
}
