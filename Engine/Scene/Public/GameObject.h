#pragma once
#include "Scene/Public/Object.h"
#include "Scene/Components/Public/Transformer.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Scene/Components/Public/Camera.h"
#include "Scene/Components/Public/Material.h"
#include "Scene/Components/Public/Light.h"

class World;

class GameObject : public Object
{
	friend class World;

	friend class Editor;

public:

	GameObject(const WEngine::WString& name = "GameObject");

	virtual ~GameObject();

	bool IsName(WEngine::WString name);

	inline const char* const GetNamePtr() const { return m_name.Data(); }

	inline WEngine::WString const GetName() const { return m_name; }

	inline const WEngine::WGuid& GetID() const { return m_id; }

	void AddGameObject(GameObject *pGameObject);

	const std::vector<GameObject*>& GetChildGameObjects() const;

	void ShowInInspector() const;

	bool HasChild() const;

	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();

public:

	static GameObject* Find(WEngine::WString name);

private:

	WEngine::WString m_name;

	const WEngine::WGuid m_id;

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