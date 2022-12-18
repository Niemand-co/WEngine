#include "pch.h"
#include "Utils/Container/Public/WHashMap.h"

namespace WEngine
{

	//template<>
	//size_t HashCode(const char* key)
	//{
	//	WGuid<const char*> guid(key);
	//	size_t index = 0;
	//	index |= ((size_t)guid.A & 0x00000000FFFFFFFF);
	//	index |= ((size_t)guid.B & 0x00000000FFFFFFFF) << 4;
	//	return index;
	//}

	template<>
	size_t HashCode(const WString& key)
	{
		WGuid<WString> guid(key);
		size_t index = 0;
		index |= ((size_t)guid.A & 0x00000000FFFFFFFF);
		index |= ((size_t)guid.B & 0x00000000FFFFFFFF) << 4;
		return index;
	}

}