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

		bool IsIntersectWithTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

		bool IsIntersectWithMesh(Mesh *pMesh);

		bool IsIntersectWithGameObject(GameObject *pGameObject);

	private:

		glm::vec3 O;

		glm::vec3 D;

	};

}