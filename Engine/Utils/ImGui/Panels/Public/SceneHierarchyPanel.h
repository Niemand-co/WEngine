#pragma once

class GWorld;

namespace WEngine
{

	class SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel();

		~SceneHierarchyPanel() = default;

		void SetScene(GWorld *pWorld);

		void DrawNodes();

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}

	private:

		GWorld *m_pScene;

	};

}