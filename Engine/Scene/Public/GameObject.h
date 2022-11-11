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

	inline const char* const GetName() const { return m_name.c_str(); }

	void AddGameObject(GameObject *pGameObject);

	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();

public:

	static GameObject* Find(std::string name);

private:

	std::string m_name;

	std::vector<GameObject*> m_sonGameObjects;

	std::vector<Component*> m_components;

};

template<typename T>
inline T* GameObject::AddComponent()
{
	T* component = new T(this);
	m_components.push_back(component);
	return component;
}

template<typename T>
inline T* GameObject::GetComponent()
{
	for (Component* component : m_components)
	{
		if (std::strcmp(WEngine::GetTypeName<T>().data(), typeid(*component).name()) == 0)
			return static_cast<T*>(component);
	}
	return nullptr;
}