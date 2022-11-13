#pragma once

class World;

namespace WEngine
{

	class SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel();

		~SceneHierarchyPanel() = default;

		void SetScene(World *pWorld);

		void DrawNodes();

		void* operator new(size_t size)
		{
			return WEngine::Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			WEngine::Allocator::Get()->Deallocate(pData);
		}

	private:

		World *m_pScene;

	};

}