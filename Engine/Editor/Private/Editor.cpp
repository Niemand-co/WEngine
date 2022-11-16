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

	std::vector<GameObject*> Editor::g_selectedObjects = std::vector<GameObject*>();

	Camera* Editor::g_pEditorCamera = nullptr;

	ray_line Editor::g_ray = {  };

	Editor::Editor()
	{
	}

	void Editor::Init()
	{
		GameObject *go = World::GetWorld()->CreateGameObject("Editor");
		Camera *pCamera = g_pEditorCamera = go->AddComponent<Camera>();
		pCamera->SetRenderer(ScriptableRenderPipeline::CreateRenderer());
	}

	void Editor::ClearSelection()
	{
		for (GameObject* gameobject : g_selectedObjects)
		{
			Mesh *pMesh = gameobject->GetComponent<MeshFilter>()->GetStaticMesh();
			for(int i = 0; i < pMesh->m_vertexCount; ++i)
				pMesh->m_pVertices[i].Color = glm::vec3(1, 1, 1);
		}
		g_selectedObjects.clear();
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

	bool Editor::IsSelected(GameObject* pGameObject)
	{
		for (GameObject* object : g_selectedObjects)
		{
			if(pGameObject == object)
				return true;
		}
		return false;
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