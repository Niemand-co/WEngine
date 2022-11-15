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

		if(result.x >= 0 && result.x <= 1.01f && result.y >= 0 && result.y <= 1.01f && result.z >= 0)
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
		glm::vec3 newD = transformer->GetWorldToLocalMatrix() * glm::vec4(O + D, 1.0f);

		Ray ray(newO, glm::normalize(newD - newO));
		MeshFilter* staticMesh = pGameObject->GetComponent<MeshFilter>();
		if (staticMesh == nullptr)
			return false;
		if (ray.IsIntersectWithMesh(staticMesh->GetStaticMesh()))
			return true;
		return false;
	}

	Ray Ray::GetClickRay(glm::vec2 ScreenPos, glm::vec3 o, glm::mat4 inverseVP)
	{
		glm::vec4 pos = glm::vec4(ScreenPos.x * 2.0f - 1.0f, ScreenPos.y * 2.0f - 1.0f, -1.0f, 1.0f);
		pos = inverseVP * pos;
		pos /= pos.w;
		Ray ray(o, glm::normalize(glm::vec3(pos) - o));

		return ray;
	}

}