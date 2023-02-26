#pragma once
#include <iostream>
#include "Utils/Allocator/Public/Allocator.h"

namespace WEngine
{

	class WString
	{
	public:

		WString();

		WString(const char* str);

		WString(const WString& string);

		~WString();

		size_t Size() const { return m_size; }

		const char* Data() const { return m_pData; }

		int32 find(const char& c) const;

		int32 find(const char* str) const;

		int32 find(const WString& string) const;

		int32 find_last_of(const char& c) const;

		WString Substr(size_t length) const;

		WString Substr(size_t start, size_t length) const;

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}

		void operator=(const WString& string)
		{
			NormalAllocator::Get()->Deallocate(m_pData);
			m_size = string.m_size;
			m_pData = (char*)NormalAllocator::Get()->Allocate(string.m_size + 1);
			memcpy(m_pData, string.m_pData, string.m_size + 1);
		}

		void operator=(const char* str)
		{
			NormalAllocator::Get()->Deallocate(m_pData);
			m_size = strlen(str);
			m_pData = (char*)NormalAllocator::Get()->Allocate(m_size + 1);
			memcpy(m_pData, str, m_size + 1);
		}

		char& operator[](const size_t& index)
		{
			return m_pData[index];
		}

		const char& operator[](const size_t& index) const
		{
			return m_pData[index];
		}

		bool operator==(WString& string) const
		{
			if(m_size != string.m_size)
				return false;

			for(size_t index = 0; index < m_size; ++index)
				if(m_pData[index] != string[index])
					return false;

			return true;
		}

		bool operator==(const char* str) const
		{
			if (m_size != strlen(str))
				return false;

			for (size_t index = 0; index < m_size; ++index)
				if (m_pData[index] != str[index])
					return false;

			return true;
		}

		WString operator+(const WString& string) const
		{
			char* newPtr = (char*)NormalAllocator::Get()->Allocate(string.m_size + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, string.m_pData, string.m_size + 1);
			WString newString = WString();
			newString.m_pData = newPtr;
			newString.m_size = m_size + string.m_size;
			return newString;
		}

		WString operator+(const char *str) const
		{
			if (str == nullptr)
			{
				return WString(*this);
			}
			size_t length = strlen(str);
			char* newPtr = (char*)NormalAllocator::Get()->Allocate(length + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, str, length + 1);
			WString newString = WString();
			newString.m_pData = newPtr;
			newString.m_size = m_size + length;
			return newString;
		}

		void operator+=(const WString& string)
		{
			char* newPtr = (char*)NormalAllocator::Get()->Allocate(string.m_size + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, string.m_pData, string.m_size + 1);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_size += string.m_size;
		}

		void operator+=(const char *str)
		{
			if (str == nullptr)
			{
				return;
			}
			size_t length = strlen(str);
			char* newPtr = (char*)NormalAllocator::Get()->Allocate(length + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, str, length + 1);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_size += length;
		}

		friend WString operator+(const char* str, const WString& string);

		friend std::ostream& operator<<(std::ostream& o, const WString& string);

	private:

		char *m_pData;

		size_t m_size;

	};

}