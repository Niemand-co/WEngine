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

	inline const char* const GetNamePtr() const { return m_name.GetData(); }

	inline const WEngine::WString& GetName() { return m_name; }

	inline const WEngine::WGuid<WEngine::WString>& GetID() const { return m_id; }

	void AddGameObject(GameObject *pGameObject);

	const WEngine::WArray<GameObject*>& GetChildGameObjects() const;

	void ShowInInspector() const;

	bool HasChild() const;

	template<typename T, typename ...Args>
	T* AddComponent(Args... args);

	template<typename T>
	T* GetComponent();

public:

	static GameObject* Find(WEngine::WString name);

private:

	WEngine::WString m_name;

	const WEngine::WGuid<WEngine::WString> m_id;

	WEngine::WArray<GameObject*> m_sonGameObjects;

	WEngine::WArray<class Component*> m_components;

	uint8 m_bStatic : 1;

};

template<typename T, typename ...Args>
inline T* GameObject::AddComponent(Args... args)
{
	T* component = new T(this, args...);
	m_components.Push(component);
	return component;
}

template<typename T>
inline T* GameObject::GetComponent()
{
	for (Component* component : m_components)
	{
		if (dynamic_cast<T*>(component) != nullptr)
			return static_cast<T*>(component);
		//if(component->type == T::type)
			//return static_cast<T*>(component);
	}
	return nullptr;
}