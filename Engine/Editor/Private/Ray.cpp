#include "pch.h"
#include "Editor/Public/Ray.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/GameObject.h"

namespace WEngine
{

	Ray::Ray(glm::vec3 o, glm::vec3 d)
		: O(o), D(d)
	{
	}

	bool Ray::IsIntersectWithTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{
		glm::vec3 ab = glm::normalize(b - a);
		glm::vec3 ac = glm::normalize(c - a);
		glm::mat3 m = glm::inverse(glm::mat3(ab, ac, -D));
		glm::vec3 result = m * (O - a);

		if(result.x >= 0 && result.x <= 1 && result.y >= 0 && result.y <= 1 && result.z >= 0)
			return true;

		return false;
	}

	bool Ray::IsIntersectWithMesh(Mesh* pMesh)
	{
		for (unsigned int i = 0; i < pMesh->m_indexCount; i += 3)
		{
			if(IsIntersectWithTriangle(pMesh->m_pVertices[pMesh->m_pIndices[i]].Position, pMesh->m_pVertices[pMesh->m_pIndices[i + 1]].Position, pMesh->m_pVertices[pMesh->m_pIndices[i + 2]].Position))
				return true;
		}
		return false;
	}

	bool Ray::IsIntersectWithGameObject(GameObject* pGameObject)
	{
		Transformer *transformer = pGameObject->GetComponent<Transformer>();

		glm::vec3 newO = transformer->GetWorldToLocalMatrix() * glm::vec4(O, 1.0f);
		glm::vec3 newD = transformer->GetWorldToLocalMatrix() * glm::vec4(D, 1.0f);

		Ray ray(newO, glm::normalize(newD - newO));
		MeshFilter* staticMesh = pGameObject->GetComponent<MeshFilter>();
		if (staticMesh == nullptr)
			return false;
		if (ray.IsIntersectWithMesh(staticMesh->GetStaticMesh()))
			return true;
		return false;
	}

}