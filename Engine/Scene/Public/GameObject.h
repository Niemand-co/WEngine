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

	bool IsName(std::string name);

	template<typename T>
	T* AddComponent();

	template<Component::ComponentType type>
	Component* GetComponent();

public:

	static GameObject* Find(std::string name);

private:

	std::string m_name;

	std::vector<Component*> m_components;

};

template<typename T>
inline T* GameObject::AddComponent()
{
	T* component = new T(this);
	m_components.push_back(component);
	return component;
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