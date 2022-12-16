#pragma once

namespace WEngine
{

	class WString
	{
	public:

		WString();

		WString(const char *str);

		WString(const WString&) = default;

		~WString();

		size_t Size() const { return m_size; }

		char* Data() const { return m_pData; }

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

		WString& operator=(const WString& string)
		{
			Allocator::Get()->Deallocate(m_pData);
			m_size = string.m_size;
			m_pData = (char*)Allocator::Get()->Allocate(string.m_size + 1);
			memcpy(m_pData, string.m_pData, string.m_size + 1);
			return *this;
		}

		WString& operator=(const char *str)
		{
			Allocator::Get()->Deallocate(m_pData);
			m_size = strlen(str);
			m_pData = (char*)Allocator::Get()->Allocate(m_size + 1);
			memcpy(m_pData, str, m_size + 1);
			return *this;
		}

		char& operator[](const size_t& index)
		{
			return m_pData[index];
		}

		const char& operator[](const size_t& index) const
		{
			return m_pData[index];
		}

		bool operator==(const WString& string)
		{
			if(m_size != string.m_size)
				return false;

			for(size_t index = 0; index < m_size; ++index)
				if(m_pData[index] != string[index])
					return false;

			return true;
		}

		bool operator==(const char* str)
		{
			if (m_size != strlen(str))
				return false;

			for (size_t index = 0; index < m_size; ++index)
				if (m_pData[index] != str[index])
					return false;

			return true;
		}

		WString&& operator+(const WString& string)
		{
			char *newPtr = (char*)Allocator::Get()->Allocate(string.m_size + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, string.m_pData, string.m_size + 1);
			WString newString = WString();
			newString.m_pData = newPtr;
			newString.m_size = m_size + string.m_size;
			return (WString&&)newString;
		}

		WString&& operator+(const char *str)
		{
			size_t length = strlen(str);
			char* newPtr = (char*)Allocator::Get()->Allocate(length + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, str, length + 1);
			WString newString = WString();
			newString.m_pData = newPtr;
			newString.m_size = m_size + length;
			return (WString&&)newString;
		}

		WString& operator+=(const WString& string)
		{
			char* newPtr = (char*)Allocator::Get()->Allocate(string.m_size + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, string.m_pData, string.m_size + 1);
			Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_size += string.m_size;
			return *this;
		}

		WString& operator+=(const char *str)
		{
			size_t length = strlen(str);
			char* newPtr = (char*)Allocator::Get()->Allocate(length + m_size + 1);
			memcpy(newPtr, m_pData, m_size);
			memcpy(newPtr + m_size, str, length + 1);
			Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_size += length;
			return *this;
		}

	private:

		char *m_pData;

		size_t m_size;

	};

}