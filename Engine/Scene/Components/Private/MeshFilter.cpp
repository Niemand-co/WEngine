#include "pch.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Render/Mesh/Public/Mesh.h"

MeshFilter::MeshFilter()
{
	m_type = Component::ComponentType::MeshFilter;
}

void MeshFilter::SetStaticMesh(Mesh* pStaticMesh)
{
	m_pStaticMesh = pStaticMesh;
}

Mesh* MeshFilter::GetStaticMesh()
{
	return m_pStaticMesh;
}
