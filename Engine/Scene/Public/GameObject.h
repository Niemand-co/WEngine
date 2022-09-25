#pragma once
#ifndef WENGINE_GAMEOBJECT_H
#define WENGINE_GAMEOBJECT_H

#include "Scene/Public/Object.h"
#include "Scene/Components/Public/Component.h"

class Transformer;
class Camera;
class MeshFilter;

class GameObject : public Object
{
public:
	GameObject();
	virtual ~GameObject();

	template<Component::ComponentType type>
	void AddComponent();
	template<Component::ComponentType type>
	Component* GetComponent();

private:
	std::vector<Component*> m_components;
};

template<Component::ComponentType type>
inline void GameObject::AddComponent()
{
	switch (type)
	{
		case Component::ComponentType::Transformer:
		{
			Component* component = new Transformer();
			m_components.push_back(component);
			break;
		}

		case Component::ComponentType::Camera:
		{
			Component* component = new Camera();
			m_components.push_back(component);
			break;
		}

		case Component::ComponentType::MeshFilter:
		{
			Component* component = new MeshFilter();
			m_components.push_back(component);
			break;
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

#endif