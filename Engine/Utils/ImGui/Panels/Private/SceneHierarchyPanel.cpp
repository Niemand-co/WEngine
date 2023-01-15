#include "pch.h"
#include "Utils/ImGui/Panels/Public/SceneHierarchyPanel.h"
#include "Scene/Public/World.h"
#include "Scene/Public/GameObject.h"
#include "Editor/Public/Editor.h"

namespace WEngine
{

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		SetScene(GWorld::GetWorld());
	}

	void SceneHierarchyPanel::SetScene(GWorld* pWorld)
	{
		m_pScene = pWorld;
	}

	void SceneHierarchyPanel::DrawNodes()
	{
		const WEngine::WArray<GameObject*>& objects = m_pScene->GetGameObjects();
		
		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		for (GameObject* pObject : objects)
		{
			ImGuiTreeNodeFlags flags = (Editor::IsSelected(pObject) ? ImGuiTreeNodeFlags_Selected : 0) | (pObject->HasChild() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf);
			if (ImGui::TreeNodeEx(pObject, flags, pObject->GetNamePtr()))
			{
				ImGui::TreePop();
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				Editor::SelectObject(pObject);
			}
		}
		ImGui::PopItemWidth();
	}

}