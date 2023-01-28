#include "pch.h"
#include "Editor/Public/Editor.h"
#include "Scene/Public/GameObject.h"
#include "Scene/Public/World.h"
#include "Render/RenderPipeline/Public/ScriptableRenderPipeline.h"
#include "Render/Mesh/Public/Mesh.h"
#include "Render/Mesh/Public/Vertex.h"

namespace WEngine
{

	int Editor::g_selectedID = 0;

	WArray<GameObject*> Editor::g_selectedObjects = WArray<GameObject*>();

	CameraComponent* Editor::g_pEditorCamera = nullptr;

	Editor::Editor()
	{
	}

	void Editor::Init()
	{
	}

	void Editor::ClearSelection()
	{
		g_selectedObjects.Clear();
	}

	void Editor::SelectObject(GameObject* pGameObject)
	{
		g_selectedObjects.Clear();
		g_selectedObjects.Push(std::move(pGameObject));
	}

	void Editor::SelectObject(const WArray<GameObject*>& gameObjects)
	{
		g_selectedObjects.Clear();
		g_selectedObjects = gameObjects;
	}

	bool Editor::IsSelected(GameObject* pGameObject)
	{
		for (GameObject* object : g_selectedObjects)
		{
			if(pGameObject == object)
				return true;
		}
		return false;
	}

	WArray<GameObject*>& Editor::GetSelectedObject()
	{
		return g_selectedObjects;
	}

	size_t Editor::GetSelectedObjectCount()
	{
		return g_selectedObjects.Size();
	}

}