#pragma once
#include "Scene/Public/Object.h"

class GWorld;

class GameObject : public Object
{
	friend class GWorld;

	friend class Editor;

public:

	GameObject(const WEngine::WString& name = "GameObject");

	virtual ~GameObject();

	bool IsName(WEngine::WString name);

	inline const char* const GetNamePtr() const { return m_name.Data(); }

	inline const WEngine::WString& const GetName() const { return m_name; }

	inline const WEngine::WGuid<WEngine::WString>& GetID() const { return m_id; }

	void AddGameObject(GameObject *pGameObject);

	const WEngine::WArray<GameObject*>& GetChildGameObjects() const;

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

	const WEngine::WGuid<WEngine::WString> m_id;

	WEngine::WArray<GameObject*> m_sonGameObjects;

	WEngine::WArray<class Component*> m_components;

};

template<typename T>
inline T* GameObject::AddComponent()
{
	T* component = new T(this);
	m_components.Push(component);
	return component;
}

template<typename T>
inline T* GameObject::GetComponent()
{
	for (Component* component : m_components)
	{
		//if (dynamic_cast<T*>(component) != nullptr)
			//return static_cast<T*>(component);
		if(component->type == T::type)
			return static_cast<T*>(component);
	}
	return nullptr;
}