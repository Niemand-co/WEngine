#include "Entity.h"
#include <iostream>

Entity::Entity()
	: isVisible(false), m_mesh(nullptr), m_material(nullptr), m_transformer(nullptr)
{

}

Entity::~Entity()
{

}

void Entity::SetVisible(bool isVisible)
{
	this->isVisible = isVisible;
}

bool Entity::GetVisible()
{
	return isVisible;
}

Mesh* Entity::GetMesh()
{
	return m_mesh;
}

Material* Entity::GetMaterial()
{
	return m_material;
}

Transformer* Entity::GetTransformer()
{
	return m_transformer;
}