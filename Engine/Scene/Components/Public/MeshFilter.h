#pragma once
#include "Scene/Components/Public/Component.h"

class StaticMesh;

class MeshFilter : public Component
{
	
	friend class GameObject;

public:

	typedef MeshFilter type;

	MeshFilter(GameObject* pGameObject);

	~MeshFilter() = default;

	virtual void ShowInInspector() override;

	void SetStaticMesh(StaticMesh *pStaticMesh);

	StaticMesh *GetStaticMesh();

private:

	StaticMesh *m_pStaticMesh;

};