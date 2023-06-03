#include "pch.h"
#include "Utils/Container/Public/WString.h"

namespace WEngine
{

	WString::WString()
	{
		CharNum = 0;
		Data = nullptr;
	}

	WString::WString(const char* str)
	{
		if (str == nullptr)
		{
			CharNum = 0;
			Data = nullptr;
			return;
		}
		CharNum = strlen(str);
		Data = (char*)GetCPUAllocator()->Allocate(CharNum + 1);
		memcpy(Data, str, CharNum + 1);
	}

	WString::WString(const WString& Other)
		: CharNum(Other.CharNum)
	{
		Data = (char*)GetCPUAllocator()->Allocate(CharNum + 1);
		memcpy(Data, Other.Data, CharNum + 1);
	}

	WString::WString(WString&& Other)
		: CharNum(Other.CharNum),
		  Data(Other.Data)
	{
		Other.Data = nullptr;
	}

	WString::~WString()
	{
		GetCPUAllocator()->Deallocate(Data);
	}

	int32 WString::find(const char& c) const
	{
		for (size_t index = 0; index < CharNum; ++index)
		{
			if(Data[index] == c)
				return index;
		}
		return -1;
	}

	int32 WString::find(const char* str) const
	{
		size_t length = strlen(str);
		for (size_t index = 0; index <= CharNum - length; ++index)
		{
			size_t start = 0;
			while (Data[start] == str[start])
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
		for (size_t index = 0; index <= CharNum - length; ++index)
		{
			size_t start = 0;
			while (Data[start] == string[start])
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
		for (int32 index = CharNum - 1; index >= 0; --index)
		{
			if(Data[index] == c)
				return index;
		}
		return -1;
	}

	WString WString::Substr(size_t length) const
	{
		RE_ASSERT(length <= CharNum, "Out of String Length.");
		WString newstring;
		newstring.CharNum = length;
		newstring.Data = (char*)GetCPUAllocator()->Allocate(length + 1);
		memcpy(newstring.Data, Data, length);
		*(newstring.Data + length) = '\0';
		return newstring;
	}

	WString WString::Substr(size_t start, size_t length) const
	{
		RE_ASSERT((start + length) <= CharNum, "Out of String Length.");
		WString newstring;
		newstring.CharNum = length;
		newstring.Data = (char*)GetCPUAllocator()->Allocate(length + 1);
		memcpy(newstring.Data, Data + start, length);
		*(newstring.Data + length) = '\0';
		return newstring;
	}

	WString operator+(const char* str, const WString& string)
	{
		size_t length = strlen(str);
		char* newPtr = (char*)GetCPUAllocator()->Allocate(length + string.CharNum + 1);
		memcpy(newPtr, str, length);
		memcpy(newPtr + length, string.Data, string.CharNum + 1);
		WString newString = WString();
		newString.Data = newPtr;
		newString.CharNum = string.CharNum + length;
		return newString;
	}

	std::ostream& operator<<(std::ostream& o, const WString& string)
	{
		o << string.Data;
		return o;
	}

}