#include "pch.h"
#include "Render/Public/SceneVisibility.h"

bool IsBoxInFrustum(WEngine::WArray<glm::vec3> Frustum, glm::vec3 BoxMin, glm::vec3 BoxMax)
{
	glm::vec3 Box[8];
	Box[0] = glm::vec3(BoxMin.x, BoxMin.y, BoxMin.z);
	Box[1] = glm::vec3(BoxMax.x, BoxMin.y, BoxMin.z);
	Box[2] = glm::vec3(BoxMax.x, BoxMax.y, BoxMin.z);
	Box[3] = glm::vec3(BoxMin.x, BoxMax.y, BoxMin.z);
	Box[4] = glm::vec3(BoxMin.x, BoxMin.y, BoxMax.z);
	Box[5] = glm::vec3(BoxMax.x, BoxMin.y, BoxMax.z);
	Box[6] = glm::vec3(BoxMax.x, BoxMax.y, BoxMax.z);
	Box[7] = glm::vec3(BoxMin.x, BoxMax.y, BoxMax.z);

	for (uint32 PointIndex = 0; PointIndex < 8; ++PointIndex)
	{
		{
			glm::vec3 Normal = glm::normalize(glm::cross(Frustum[1] - Frustum[0], Frustum[2] - Frustum[1]));
			glm::vec3 Line = glm::normalize(Box[PointIndex] - Frustum[0]);
			if (glm::dot(Normal, Line) < 0)
				return false;
		}

		for (uint32 FaceIndex = 0; FaceIndex < 4; ++FaceIndex)
		{
			uint32 index = (FaceIndex + 1) % 4;
			glm::vec3 Normal = glm::normalize(glm::cross(Frustum[index] - Frustum[FaceIndex], Frustum[index + 4] - Frustum[index]));
			glm::vec3 Line = glm::normalize(Box[PointIndex] - Frustum[FaceIndex]);
			if (glm::dot(Normal, Line) < 0)
				return false;
		}

		{
			glm::vec3 Normal = glm::normalize(glm::cross(Frustum[5] - Frustum[4], Frustum[6] - Frustum[5]));
			glm::vec3 Line = glm::normalize(Box[PointIndex] - Frustum[4]);
			if (glm::dot(Normal, Line) < 0)
				return false;
		}
	}
}

bool IsSphereInFrustum(WEngine::WArray<glm::vec3> Frustum, glm::vec3 Center, float Radius)
{
	return false;
}