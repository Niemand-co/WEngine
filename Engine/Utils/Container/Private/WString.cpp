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
		if (str == nullptr)
		{
			m_size = 0;
			m_pData = nullptr;
			return;
		}
		m_size = strlen(str);
		m_pData = (char*)NormalAllocator::Get()->Allocate(m_size + 1);
		memcpy(m_pData, str, m_size + 1);
	}

	WString::WString(const WString& string)
	{
		m_size = string.m_size;
		m_pData = (char*)NormalAllocator::Get()->Allocate(m_size + 1);
		memcpy(m_pData, string.m_pData, m_size + 1);
	}

	WString::~WString()
	{
		NormalAllocator::Get()->Deallocate(m_pData);
	}

	int32 WString::find(const char& c) const
	{
		for (size_t index = 0; index < m_size; ++index)
		{
			if(m_pData[index] == c)
				return index;
		}
		return -1;
	}

	int32 WString::find(const char* str) const
	{
		size_t length = strlen(str);
		for (size_t index = 0; index <= m_size - length; ++index)
		{
			size_t start = 0;
			while (m_pData[start] == str[start])
			{
				++start;
			}
			if(start == (length - 1))
				return index;
		}
		return -1;
	}

	int32 WString::find(const WString& string) const
	{
		size_t length = string.Size();
		for (size_t index = 0; index <= m_size - length; ++index)
		{
			size_t start = 0;
			while (m_pData[start] == string[start])
			{
				++start;
			}
			if (start == (length - 1))
				return index;
		}
		return -1;
	}

	int32 WString::find_last_of(const char& c) const
	{
		for (int32 index = m_size - 1; index >= 0; --index)
		{
			if(m_pData[index] == c)
				return index;
		}
		return -1;
	}

	WString WString::Substr(size_t length) const
	{
		RE_ASSERT(length <= m_size, "Out of String Length.");
		WString newstring;
		newstring.m_size = length;
		newstring.m_pData = (char*)NormalAllocator::Get()->Allocate(length + 1);
		memcpy(newstring.m_pData, m_pData, length);
		*(newstring.m_pData + length) = '\0';
		return newstring;
	}

	WString WString::Substr(size_t start, size_t length) const
	{
		RE_ASSERT((start + length) <= m_size, "Out of String Length.");
		WString newstring;
		newstring.m_size = length;
		newstring.m_pData = (char*)NormalAllocator::Get()->Allocate(length + 1);
		memcpy(newstring.m_pData, m_pData + start, length);
		*(newstring.m_pData + length) = '\0';
		return newstring;
	}

	WString operator+(const char* str, const WString& string)
	{
		size_t length = strlen(str);
		char* newPtr = (char*)NormalAllocator::Get()->Allocate(length + string.m_size + 1);
		memcpy(newPtr, str, length);
		memcpy(newPtr + length, string.m_pData, string.m_size + 1);
		WString newString = WString();
		newString.m_pData = newPtr;
		newString.m_size = string.m_size + length;
		return newString;
	}

	std::ostream& operator<<(std::ostream& o, const WString& string)
	{
		o << string.m_pData;
		return o;
	}

}