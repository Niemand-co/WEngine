#include "pch.h"
#include "Editor/Public/Ray.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/GameObject.h"
#include "Editor/Public/Editor.h"

namespace WEngine
{

	Ray::Ray(glm::vec3 o, glm::vec3 d)
		: O(o), D(d)
	{
	}

	bool Ray::IsIntersectWithTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{
		glm::vec3 normal = glm::normalize(glm::cross(b - a, c - b));

		float isVertical = dot(normal, D);
		if( isVertical < 0.000001f && isVertical > -0.000001f )
			return false;

		float t = glm::dot(a - O, normal) / glm::dot(normal, D);

		if(t < 0.0f)
			return false;

		glm::vec3 p = O + D * t;
		glm::vec3 abp = glm::cross(b - a, p - a);
		glm::vec3 bcp = glm::cross(c - b, p - b);
		glm::vec3 cap = glm::cross(a - c, p - c);

		if (glm::dot(abp, bcp) > 0.0f && glm::dot(bcp, cap) > 0.0f)
		{
			return true;
		}

		return false;
	}

	bool Ray::IsIntersectWithMesh(Mesh* pMesh)
	{
		for (unsigned int i = 0; i < pMesh->m_indexCount; i += 3)
		{
			if (IsIntersectWithTriangle(pMesh->m_pVertices[pMesh->m_pIndices[i]].Position, pMesh->m_pVertices[pMesh->m_pIndices[i + 1]].Position, pMesh->m_pVertices[pMesh->m_pIndices[i + 2]].Position))
			{
				pMesh->m_pVertices[pMesh->m_pIndices[i]].Color = glm::vec3(1, 0, 0);
				pMesh->m_pVertices[pMesh->m_pIndices[i + 1]].Color = glm::vec3(1, 0, 0);
				pMesh->m_pVertices[pMesh->m_pIndices[i + 2]].Color = glm::vec3(1, 0, 0);
				return true;
			}
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
		if (IsIntersectWithMesh(staticMesh->GetStaticMesh()))
			return true;
		return false;
	}

	Ray Ray::GetClickRay(glm::vec2 ScreenPos, glm::vec3 o, glm::mat4 inverseV, glm::mat4 inverseP)
	{
		glm::vec4 pos = glm::vec4(ScreenPos.x * 2.0f - 1.0f, ScreenPos.y * 2.0f - 1.0f, 1.0f, 1.0f);
		pos = inverseP * pos;
		pos /= pos.w;
		pos = inverseV * pos;
		Ray ray(o, glm::normalize(glm::vec3(pos) - o));
		Editor::g_ray.O.Position = o;
		Editor::g_ray.D.Position = 10000.0f * glm::normalize(glm::vec3(pos) - o);
		std::cout<<"("<<Editor::g_ray.D.Position.x<<","<<Editor::g_ray.D.Position.y<<","<<Editor::g_ray.D.Position.z<<")" << std::endl;
		return ray;
	}

}