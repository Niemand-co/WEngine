#include "pch.h"
#include "Utils/Allocator/Public/Allocator.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace WEngine
{
	Allocator *Allocator::g_pInstance = nullptr;

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

	Allocator::Allocator()
	{
		for (unsigned int i = 0; i < 8; ++i)
		{
			::new (&freeLists[i]) WQueue<void*>();
		}
	}

	Allocator::~Allocator()
	{
	}

	void Allocator::Init(Backend backend)
	{
		switch (backend)
		{
		case WEngine::Backend::None:
			g_pInstance = new Allocator();
			return;
		case WEngine::Backend::Vulkan:
			g_pInstance = new Vulkan::VulkanAllocator();
			return;
		default:
			RE_ASSERT(false, "Error Backend Allocator.");
			return;
		}
	}

	Allocator* Allocator::Get()
	{
		if (g_pInstance == nullptr)
		{
			g_pInstance = new Vulkan::VulkanAllocator();
		}
		return g_pInstance;
	}

	void* Allocator::Allocate(size_t size)
	{
		if (size <= 0)
		{
			return nullptr;
		}
		else if (size > 1024)
		{
			BYTE *block = (BYTE*)malloc(size + sizeof(BYTE));
			::new (block) BYTE(0);
			return block + 1;
		}
		else
		{
			size_t index = GetIndex(size);
			size_t blockSize = GetBlockSize(index);
			if (freeLists[index].Empty())
			{
				size_t blockCount = 4096 / blockSize;
				BYTE *data = (BYTE*)malloc(blockCount * blockSize + blockCount * sizeof(BYTE));
				for (size_t i = 0; i < blockCount; ++i)
				{
					::new (data) BYTE(index + 1);
					freeLists[index].Push(data + 1);
					data += (blockSize + 1);
				}
			}
			void* block = freeLists[index].Pop();
			return block;
		}
	}

	void Allocator::Deallocate(void* pBlock)
	{
		if(pBlock == nullptr)
			return;
		BYTE index = *((BYTE*)pBlock - 1);
		if (index == 0)
		{ 
			free ((void*)((BYTE*)pBlock - 1));
		}
		else
		{
			index -= 1;
			freeLists[index].Push(pBlock);
		}
	}

	void* Allocator::Reallocate(void* pBlock, size_t newSize)
	{
		BYTE index = *((BYTE*)pBlock - 1);
		if (index == 0)
		{
			BYTE* newBlock = (BYTE*)realloc((BYTE*)pBlock - 1, newSize + sizeof(BYTE));
			return newBlock + 1;
		}
		if(newSize <= 4096 && index - 1 == GetIndex(newSize))
			return pBlock;
		BYTE *newBlock = (BYTE*)Allocate(newSize);
		std::memcpy(newBlock, pBlock, GetBlockSize(index - 1));
		Deallocate(pBlock);
		return newBlock;
	}

}