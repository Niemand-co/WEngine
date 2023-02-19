#pragma once

namespace WEngine
{

	class NamingSystem
	{
	public:

		struct NameNode
		{
			char c;
			WEngine::WArray<NameNode*> next;

			NameNode() = default;

			NameNode(const char& c)
				: c(c)
			{
				next = WEngine::WArray<NameNode*>();
			}

			void* operator new(size_t size)
			{
				return NormalAllocator::Get()->Allocate(size);
			}

			void operator delete(void* pData)
			{
				NormalAllocator::Get()->Deallocate(pData);
			}

			void AddNode(const char& c)
			{
				NameNode *pNode = new NameNode();
				pNode->c = c;
				next.Push(pNode);
			}
		};

		bool AddName(const WEngine::WString& name) const;

		bool IsNameExist(const WEngine::WString& name) const;

	protected:

		NameNode *m_nameTree = new NameNode();

	};

}