#include "pch.h"
#include "Editor/Public/Editor.h"
#include "Scene/Public/GameObject.h"

namespace WEngine
{

	std::vector<GameObject*> Editor::g_selectedObjects = std::vector<GameObject*>();

	Editor::Editor()
	{
	}

	void Editor::SelectObject(GameObject* pGameObject)
	{
		g_selectedObjects.clear();
		g_selectedObjects.push_back(std::move(pGameObject));
	}

	void Editor::SelectObject(const std::vector<GameObject*>& gameObjects)
	{
		g_selectedObjects.clear();
		g_selectedObjects = gameObjects;
	}

	std::vector<GameObject*>& Editor::GetSelectedObject()
	{
		return g_selectedObjects;
	}

	size_t Editor::GetSelectedObjectCount()
	{
		return g_selectedObjects.size();
	}

}