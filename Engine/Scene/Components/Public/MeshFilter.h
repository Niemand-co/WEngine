#pragma once
#include "Scene/Components/Public/Component.h"

class WStaticMesh;

class MeshFilter : public Component
{
	
	friend class GameObject;

public:

	typedef MeshFilter type;

	MeshFilter(GameObject* pGameObject);

	~MeshFilter() = default;

	virtual void ShowInInspector() override;

	void SetStaticMesh(WStaticMesh*pStaticMesh);

	WStaticMesh *GetStaticMesh();

private:

	WStaticMesh *m_pStaticMesh;

};