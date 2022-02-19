#pragma once
#ifndef SR_ENTITY_H
#define SR_ENTITY_H

#include "Components/Material.h"
#include "Components/Mesh.h"
#include "Components/Transformer.h"
#include "Utils/Tools.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void SetVisible(bool isVisible);
	virtual bool GetVisible();

	virtual Mesh* GetMesh();
	virtual Material* GetMaterial();
	virtual Transformer* GetTransformer();

	template<typename T>
	void AddComponent(void* component);

	template<typename T>
	T* AddComponent();
private:
	bool isVisible;
	Mesh* m_mesh;
	Material* m_material;
	Transformer* m_transformer;
};

template<typename T>
void Entity::AddComponent(void* component)
{
	if (isSameTpye(static_cast<T*>(component), m_mesh))
	{
		m_mesh = static_cast<Mesh*>(component);
		return;
	}
	else if (isSameTpye(static_cast<T*>(component), m_material))
	{
		m_material = static_cast<Material*>(component);
		return;
	}
	else if (isSameTpye(static_cast<T*>(component), m_transformer))
	{
		m_transformer = static_cast<Transformer*>(component);
		return;
	}
	else
	{
		std::cout<<"Error Type"<<std::endl;
		exit(0);
	}
}

template<typename T>
T* Entity::AddComponent()
{
	void* component = new T();
	if (isSameTpye(static_cast<T*>(component), m_mesh))
	{
		m_mesh = static_cast<Mesh*>(component);
		return static_cast<T*>(component);
	}
	else if (isSameTpye(static_cast<T*>(component), m_material))
	{
		m_material = static_cast<Material*>(component);
		return static_cast<T*>(component);
	}
	else if (isSameTpye(static_cast<T*>(component), m_transformer))
	{
		m_transformer = static_cast<Transformer*>(component);
		return static_cast<T*>(component);
	}
	else
	{
		std::cout << "Error Type" << std::endl;
		exit(0);
	}
	return nullptr;
}

#endif