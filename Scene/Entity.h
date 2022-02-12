#pragma once
#ifndef SR_ENTITY_H
#define SR_ENTITY_H

#include "Components/Material.h"
#include "Components/Mesh.h"
#include "Utils/Tools.h"

class Entity
{
public:
	Entity();
	~Entity();

	virtual void SetVisible(bool isVisible);
	virtual bool GetVisible();

	virtual Mesh* GetMesh();
	virtual Material* GetMaterial();

	template<typename T>
	void AddComponent(void* component);

private:
	bool isVisible;
	Mesh* m_mesh;
	Material* m_material;
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
	else
	{
		std::cout<<"Error Type"<<std::endl;
		exit(0);
	}
}

#endif