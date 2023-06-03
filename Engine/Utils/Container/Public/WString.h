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
		WString(const WString& Other);
		explicit WString(WString&& Other);

		WString& operator=(const WString& Other)
		{
			GetCPUAllocator()->Deallocate(Data);
			CharNum = Other.CharNum;
			Data = (char*)GetCPUAllocator()->Allocate(Other.CharNum + 1);
			memcpy(Data, Other.Data, Other.CharNum + 1);
			return *this;
		}

		WString& operator=(WString&& Other)
		{
			CharNum = Other.CharNum;
			Data = Other.Data;
			Other.Data = nullptr;
			return *this;
		}

		~WString();

		size_t Size() const { return CharNum; }

		const char* GetData() const { return Data; }

		int32 find(const char& c) const;

		int32 find(const char* str) const;

		int32 find(const WString& string) const;

		int32 find_last_of(const char& c) const;

		WString Substr(size_t length) const;

		WString Substr(size_t start, size_t length) const;

		void* operator new(size_t size)
		{
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

		void operator=(const char* str)
		{
			GetCPUAllocator()->Deallocate(Data);
			CharNum = strlen(str);
			Data = (char*)GetCPUAllocator()->Allocate(CharNum + 1);
			memcpy(Data, str, CharNum + 1);
		}

		char& operator[](const size_t& index)
		{
			return Data[index];
		}

		const char& operator[](const size_t& index) const
		{
			return Data[index];
		}

		bool operator==(WString& string) const
		{
			if(CharNum != string.CharNum)
				return false;

			for(size_t index = 0; index < CharNum; ++index)
				if(Data[index] != string[index])
					return false;

			return true;
		}

		bool operator==(const char* str) const
		{
			if (CharNum != strlen(str))
				return false;

			for (size_t index = 0; index < CharNum; ++index)
				if (Data[index] != str[index])
					return false;

			return true;
		}

		WString operator+(const WString& string) const
		{
			char* newPtr = (char*)GetCPUAllocator()->Allocate(string.CharNum + CharNum + 1);
			memcpy(newPtr, Data, CharNum);
			memcpy(newPtr + CharNum, string.Data, string.CharNum + 1);
			WString newString = WString();
			newString.Data = newPtr;
			newString.CharNum = CharNum + string.CharNum;
			return newString;
		}

		WString operator+(const char *str) const
		{
			if (str == nullptr)
			{
				return WString(*this);
			}
			size_t length = strlen(str);
			char* newPtr = (char*)GetCPUAllocator()->Allocate(length + CharNum + 1);
			memcpy(newPtr, Data, CharNum);
			memcpy(newPtr + CharNum, str, length + 1);
			WString newString = WString();
			newString.Data = newPtr;
			newString.CharNum = CharNum + length;
			return newString;
		}

		void operator+=(const WString& string)
		{
			char* newPtr = (char*)GetCPUAllocator()->Allocate(string.CharNum + CharNum + 1);
			memcpy(newPtr, Data, CharNum);
			memcpy(newPtr + CharNum, string.Data, string.CharNum + 1);
			GetCPUAllocator()->Deallocate(Data);
			Data = newPtr;
			CharNum += string.CharNum;
		}

		void operator+=(const char *str)
		{
			if (str == nullptr)
			{
				return;
			}
			size_t length = strlen(str);
			char* newPtr = (char*)GetCPUAllocator()->Allocate(length + CharNum + 1);
			memcpy(newPtr, Data, CharNum);
			memcpy(newPtr + CharNum, str, length + 1);
			GetCPUAllocator()->Deallocate(Data);
			Data = newPtr;
			CharNum += length;
		}

		friend WString operator+(const char* str, const WString& string);

		friend std::ostream& operator<<(std::ostream& o, const WString& string);

	private:

		char *Data;

		size_t CharNum;

	};

}