#include "pch.h"
#include "Utils/Container/Public/WString.h"

namespace WEngine
{

	WString::WString()
	{
		m_size = 0;
		m_pData = nullptr;
	}

	WString::WString(const char* str)
	{
		m_size = strlen(str);
		m_pData = (char*)Allocator::Get()->Allocate(m_size + 1);
		memcpy(m_pData, str, m_size + 1);
	}

	WString::~WString()
	{
		Allocator::Get()->Deallocate(m_pData);
	}

}