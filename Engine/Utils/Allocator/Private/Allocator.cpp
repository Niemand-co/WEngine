#include "pch.h"
#include "Utils/Allocator/Public/Allocator.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace WEngine
{
	template<>
	Allocator<8> *Allocator<8>::g_pInstance = new Vulkan::VulkanAllocator();

	BYTE GetIndex(size_t blockSize)
	{
		if(blockSize <= 32)
			return 0;
		else if (blockSize <= 64)
			return 1;
		else if (blockSize <= 128)
			return 2;
		else if (blockSize <= 256)
			return 3;
		else if (blockSize <= 512)
			return 4;
		else if (blockSize <= 1024)
			return 5;
		else if (blockSize <= 2048)
			return 6;
		else if (blockSize <= 4096)
			return 7;
		RE_ASSERT(false, "Error Size.");
	}

	size_t GetBlockSize(BYTE index)
	{
		switch (index)
		{
		case 0:
			return 32;
		case 1:
			return 64;
		case 2:
			return 128;
		case 3:
			return 256;
		case 4:
			return 512;
		case 5:
			return 1024;
		case 6:
			return 2048;
		case 7:
			return 4096;
		default:
			RE_ASSERT(false, "Out of Range.");
			return 0;
		}
	}



}