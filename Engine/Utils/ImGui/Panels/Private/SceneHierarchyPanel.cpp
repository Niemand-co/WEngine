#include "pch.h"
#include "Utils/ImGui/Panels/Public/SceneHierarchyPanel.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Editor/Public/Editor.h"

namespace WEngine
{

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		SetScene(World::GetWorld());
	}

	void SceneHierarchyPanel::SetScene(World* pWorld)
	{
		m_pScene = pWorld;
	}

	void SceneHierarchyPanel::DrawNodes()
	{
		const std::vector<GameObject*>& objects = m_pScene->GetGameObjects();
		
		for (GameObject* pObject : objects)
		{
			ImGuiTreeNodeFlags flags = (Editor::IsSelected(pObject) ? ImGuiTreeNodeFlags_Selected : 0) | (pObject->HasChild() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf);
			if (ImGui::TreeNodeEx(pObject, flags, pObject->GetName()))
			{
				ImGui::TreePop();
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				Editor::SelectObject(pObject);
			}
		}
		
	}

}