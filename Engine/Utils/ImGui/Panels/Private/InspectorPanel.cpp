#include "pch.h"
#include "Utils/ImGui/Panels/Public/InspectorPanel.h"
#include "Scene/Public/GameObject.h"
#include "Editor/Public/Editor.h"

namespace WEngine
{

	InspectorPanel::InspectorPanel()
	{
	}

	void InspectorPanel::DrawGameObject()
	{
		if(Editor::GetSelectedObject().size() == 0)
			return;
		GameObject *pCurrentObject = Editor::GetSelectedObject()[0];

		ImGui::Text(pCurrentObject->GetName());
		if (ImGui::BeginChild("Transformer"))
		{
			pCurrentObject->ShowInInspector();
			ImGui::EndChild();
		}
	}

}