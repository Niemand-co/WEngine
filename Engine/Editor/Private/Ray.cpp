#include "pch.h"
#include "Editor/Public/Ray.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Components/Public/TransformComponent.h"
#include "Scene/Components/Public/MeshFilter.h"
#include "Editor/Public/Editor.h"

namespace WEngine
{

	Ray::Ray(glm::vec3 o, glm::vec3 d)
		: O(o), D(d)
	{
	}

	bool Ray::IsIntersectWithTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
	{
		glm::vec3 normal = glm::normalize(glm::cross(b - a, c - b));

		float isVertical = dot(normal, D);
		if( isVertical < 0.000001f && isVertical > -0.000001f )
			return false;

		float t = glm::dot(a - O, normal) / isVertical;

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

	bool Ray::IsIntersectWithMesh(StaticMesh* pMesh)
	{
		for (unsigned int i = 0; i < pMesh->m_indexCount; i += 3)
		{
			if (IsIntersectWithTriangle(pMesh->m_vertices[pMesh->m_pIndices[i]].Position, pMesh->m_vertices[pMesh->m_pIndices[i + 1]].Position, pMesh->m_vertices[pMesh->m_pIndices[i + 2]].Position))
			{
				return true;
			}
		}
		return false;
	}

	bool Ray::IsIntersectWithGameObject(GameObject* pGameObject)
	{
		TransformComponent *transformer = pGameObject->GetComponent<TransformComponent>();

		glm::vec3 newO = transformer->GetWorldToLocalMatrix() * glm::vec4(O, 1.0f);
		glm::vec3 newD = transformer->GetWorldToLocalMatrix() * glm::vec4(O + D, 1.0f);

		Ray ray(newO, glm::normalize(newD - newO));
		MeshFilter* staticMesh = pGameObject->GetComponent<MeshFilter>();
		if (staticMesh == nullptr)
			return false;
		if(!ray.IsIntersectionWithCube(staticMesh->GetStaticMesh()->m_boundingBox.BoxMin, staticMesh->GetStaticMesh()->m_boundingBox.BoxMax))
			return false;
		if (ray.IsIntersectWithMesh(staticMesh->GetStaticMesh()))
			return true;
		return false;
	}

	bool Ray::IsIntersectionWithCube(const glm::vec3& cMin, const glm::vec3& cMax)
	{
		if(D.x == 0.0f && (O.x < cMin.x || O.x > cMax.x))
			return false;
		float XslabMin = (cMin.x - O.x) / D.x;
		float XslabMax = (cMax.x - O.x) / D.x;
		
		if(XslabMax < 0.0f && XslabMin < 0.0f)
			return false;

		if (D.y == 0.0f && (O.y < cMin.y || O.y > cMax.y))
			return false;
		float YslabMin = (cMin.y - O.y) / D.y;
		float YslabMax = (cMax.y - O.y) / D.y;

		if (YslabMax < 0.0f && YslabMin < 0.0f)
			return false;

		if (D.z == 0.0f && (O.z < cMin.z || O.z > cMax.z))
			return false;
		float ZslabMin = (cMin.z - O.z) / D.z;
		float ZslabMax = (cMax.z - O.z) / D.z;

		if (ZslabMax < 0.0f && ZslabMin < 0.0f)
			return false;

		if (XslabMax < XslabMin)
		{
			float tmp = XslabMin;
			XslabMin = XslabMax;
			XslabMax = tmp;
		}

		if (YslabMax < YslabMin)
		{
			float tmp = YslabMin;
			YslabMin = YslabMax;
			YslabMax = tmp;
		}

		if (ZslabMax < ZslabMin)
		{
			float tmp = ZslabMin;
			ZslabMin = ZslabMax;
			ZslabMax = tmp;
		}

		if(YslabMax < XslabMin || YslabMin > XslabMax)
			return false;

		if (ZslabMax < XslabMin || ZslabMin > XslabMax)
			return false;

		if (YslabMax < ZslabMin || YslabMin > ZslabMax)
			return false;
			
		return true;
	}

	Ray Ray::GetClickRay(glm::vec2 ScreenPos, glm::vec3 o, glm::mat4 inverseV, glm::mat4 inverseP)
	{
		glm::vec4 pos = glm::vec4(ScreenPos.x * 2.0f - 1.0f, ScreenPos.y * 2.0f - 1.0f, 1.0f, 1.0f);
		pos = inverseP * pos;
		pos /= pos.w;
		pos = inverseV * pos;
		Ray ray(o, glm::normalize(glm::vec3(pos) - o));
		return ray;
	}

}