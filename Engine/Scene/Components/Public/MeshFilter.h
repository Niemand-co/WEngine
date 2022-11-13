#pragma once
#include "Scene/Components/Public/Component.h"

class Mesh;

class MeshFilter : public Component
{
	
	friend class GameObject;

public:

	typedef MeshFilter type;

	MeshFilter(GameObject* pGameObject);

	~MeshFilter() = default;

	virtual void ShowInInspector() override;

	void SetStaticMesh(Mesh *pStaticMesh);

	Mesh* GetStaticMesh();

private:

	Mesh *m_pStaticMesh;

};