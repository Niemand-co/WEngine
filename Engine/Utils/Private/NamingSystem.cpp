#include "pch.h"
#include "Utils/Public/NamingSystem.h"

namespace WEngine
{

	bool NamingSystem::AddName(const WEngine::WString& name) const
	{
		NameNode *curNode = m_nameTree;
		NameNode *lastNode = m_nameTree;
		size_t index = 0;
		while (curNode)
		{
			if(index == name.Size() - 1)
				return true;

			lastNode = curNode;
			curNode = nullptr;
			for (NameNode*& node : lastNode->next)
			{
				if (node->c == name[index])
				{
					curNode = node;
					++index;
					break;
				}
			}
		}

		for (; index < name.Size(); ++index)
		{
			lastNode->AddNode(name[index]);
			lastNode = lastNode->next[lastNode->next.Size() - 1];
		}

		return false;
	}

	bool NamingSystem::IsNameExist(const WEngine::WString& name) const
	{
		NameNode* curNode = m_nameTree;
		size_t index = 0;
		while (curNode)
		{
			if (index == name.Size() - 1)
				return true;

			NameNode* cur = curNode;
			curNode = nullptr;
			for (NameNode*& node : cur->next)
			{
				if (node->c == name[index])
				{
					curNode = node;
					break;
				}
			}
		}
		return false;
	}

}