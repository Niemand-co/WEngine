#pragma once

class Mesh;
class GameObject;

namespace WEngine
{

	class Ray
	{
	public:

		Ray(glm::vec3 o, glm::vec3 d);

		~Ray() = default;

		bool IsIntersectWithTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

		bool IsIntersectWithMesh(Mesh *pMesh);

		bool IsIntersectWithGameObject(GameObject *pGameObject);

		bool IsIntersectionWithCube(const glm::vec3& cMin, const glm::vec3& cMax);

	public:

		static Ray GetClickRay(glm::vec2 ScreenPos, glm::vec3 o, glm::mat4 inverseV, glm::mat4 inverseP);

	private:

		glm::vec3 O;

		glm::vec3 D;

	};

}