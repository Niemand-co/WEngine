#pragma once
#include "Render/Mesh/Public/Vertex.h"

class GameObject;
class Camera;

struct ray_line
{
	Vertex O;
	Vertex D;
};

namespace WEngine
{

	class Editor
	{
	public:

		Editor();

		~Editor() = default;

		static void Init();

		static void ClearSelection();

		static void SelectObject(GameObject *pGameObject);

		static void SelectObject(const std::vector<GameObject*>& gameObjects);

		static bool IsSelected(GameObject *pGameObject);

		static std::vector<GameObject*>& GetSelectedObject();

		static size_t GetSelectedObjectCount();

	public:

		static int g_selectedID;

		static std::vector<GameObject*> g_selectedObjects;

		static Camera *g_pEditorCamera;

		static ray_line g_ray;

	};

}