#pragma once
#include "Scene/Components/Public/Component.h"

class Mesh;

class MeshFilter : public Component
{
public:

	MeshFilter(GameObject* pGameObject);

	~MeshFilter() = default;

	void SetStaticMesh(Mesh *pStaticMesh);

	Mesh* GetStaticMesh();

private:

	Mesh *m_pStaticMesh;

};