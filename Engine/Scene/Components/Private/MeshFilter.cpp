#include "pch.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Render/Mesh/Public/Mesh.h"

MeshFilter::MeshFilter(GameObject* pGameObject)
	: Component(pGameObject)
{
}

void MeshFilter::ShowInInspector()
{
}

void MeshFilter::SetStaticMesh(WStaticMesh* pStaticMesh)
{
	m_pStaticMesh = pStaticMesh;
}

WStaticMesh* MeshFilter::GetStaticMesh()
{
	return m_pStaticMesh;
}
