#pragma once
#include "Scene/Public/Object.h"
#include "Scene/Components/Public/Transformer.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Scene/Components/Public/Camera.h"
#include "Scene/Components/Public/Material.h"

class World;

class GameObject : public Object
{
	friend class World;

public:

	GameObject(std::string name = "GameObject");

	virtual ~GameObject();

	bool IsName(std::string name);

	inline const char* const GetNamePtr() const { return m_name.data(); }

	inline std::string const GetName() const { return m_name; }

	void AddGameObject(GameObject *pGameObject);

	const std::vector<GameObject*>& GetChildGameObjects() const;

	void ShowInInspector() const;

	bool HasChild() const;

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