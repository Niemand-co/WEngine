#pragma once

#include "Scene/Public/Object.h"
#include "Scene/Components/Public/Transformer.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Scene/Components/Public/Camera.h"
#include "Scene/Components/Public/Material.h"

class World;

class GameObject : public Object
{
public:

	GameObject(std::string name = "GameObject");

	virtual ~GameObject();

	template<Component::ComponentType type>
	Component* AddComponent();

	template<Component::ComponentType type>
	Component* GetComponent();

private:

	std::string m_name;

	std::vector<Component*> m_components;

};

template<Component::ComponentType type>
inline Component* GameObject::AddComponent()
{
	switch (type)
	{
		case Component::ComponentType::Transformer:
		{
			Component* pComponent = new Transformer();
			m_components.push_back(pComponent);
			return pComponent;
		}

		case Component::ComponentType::Camera:
		{
			Component* pComponent = new Camera();
			m_components.push_back(pComponent);
			World::GetWorld()->AddCamera(static_cast<Camera*>(pComponent));
			return pComponent;
		}

		case Component::ComponentType::MeshFilter:
		{
			Component* pComponent = new MeshFilter();
			m_components.push_back(pComponent);
			return pComponent;
		}

		case Component::ComponentType::Material
		{
			Component* pComponent = new Material();
			m_components.push_back(pComponent);
			return pComponent;
		}

		default:
		{
			RE_ERROR("Not indicate component type or error type.");
		}
	}
}

template<Component::ComponentType type>
inline Component* GameObject::GetComponent()
{
	for (Component* component : m_components)
	{
		if (component->IsType(type))
			return component;
	}
	return nullptr;
}