#include "pch.h"
#include "Utils/Allocator/Public/Allocator.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace WEngine
{
	Allocator *Allocator::g_pInstance = nullptr;

	size_t GetIndex(size_t blockSize)
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

	size_t GetBlockSize(size_t index)
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
			heads[i] = nullptr;
			freeLists[i] = nullptr;
			sizes[i] = 0;
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
			g_pInstance = new Allocator();
		}
		return g_pInstance;
	}

	void* Allocator::Allocate(size_t size)
	{
		if (size <= 0)
		{
			return nullptr;
		}
		else if (size >= 4096)
		{
			return (void*)malloc(size);
		}
		else
		{
			size_t index = GetIndex(size);
			size_t blockSize = GetBlockSize(index);
			if (freeLists[index] != nullptr)
			{
				Block* block = freeLists[index];
				freeLists[index] = block->prev;
				block->~Block();
				return block;
			}
			if (sizes[index] == 0)
			{
				sizes[index] += (4096 / blockSize) * blockSize * 4;
				heads[index] = (BYTE*)malloc(sizes[index]);
			}
			void *block = heads[index];
			heads[index] += blockSize;
			sizes[index] -= blockSize;
			return block;
		}
	}

	void Allocator::Deallocate(void* pBlock, size_t size)
	{
		if (size >= 4096)
		{
			free(pBlock);
		}
		else
		{
			size_t index = GetIndex(size);
			Block *block = (Block*)pBlock;
			::new (block) Block();
			block->prev = freeLists[index];
			freeLists[index] = block;
		}
	}

	void* Allocator::Reallocate(void* pBlock, size_t originSize, size_t newSize)
	{
		size_t index = GetIndex(originSize);
		if(index == GetIndex(newSize))
			return pBlock;
		void* newBlock = Allocate(newSize);
		return newBlock;
	}

}