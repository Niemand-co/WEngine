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

		static void SelectObject(GameObject *pGameObject);

		static void SelectObject(const std::vector<GameObject*>& gameObjects);

		static std::vector<GameObject*>& GetSelectedObject();

		static size_t GetSelectedObjectCount();

	public:

		static std::vector<GameObject*> g_selectedObjects;

		static Camera *g_pEditorCamera;

	};

}