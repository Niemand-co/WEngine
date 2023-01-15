#pragma once
#include "Render/Mesh/Public/Vertex.h"

class GameObject;
class CameraComponent;

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

		static void SelectObject(const WArray<GameObject*>& gameObjects);

		static bool IsSelected(GameObject *pGameObject);

		static WArray<GameObject*>& GetSelectedObject();

		static size_t GetSelectedObjectCount();

	public:

		static int g_selectedID;

		static WArray<GameObject*> g_selectedObjects;

		static CameraComponent *g_pEditorCamera;

	};

}