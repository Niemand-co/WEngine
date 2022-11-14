#pragma once

class GameObject;
class Camera;

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

	};

}