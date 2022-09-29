#include "pch.h"
#include "Utils/Allocator/Public/Allocator.h"
#include "Platform/Vulkan/Allocator/Public/VulkanAllocator.h"

namespace WEngine
{
	Allocator *Allocator::g_pInstance = nullptr;

	Allocator::Allocator()
	{
		head = (BYTE*)malloc(4086);
		for(unsigned int i = 0; i < 5; ++i)
			lists[i] = nullptr;
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
		if (size <= 32)
		{
			if (lists[0] != nullptr)
			{
				Block *block = lists[0];
				lists[0] = block->prev;
				block->~Block();
				return (void*)block;
			}
			else
			{
				void *result = head;
				head = head + 32;
				return result;
			}
		}
		else if (size <= 64)
		{
			if (lists[1] != nullptr)
			{
				Block* block = lists[1];
				lists[1] = block->prev;
				block->~Block();
				return (void*)block;
			}
			else
			{
				void* result = head;
				head = head + 64;
				return result;
			}
		}
		else if (size <= 96)
		{
			if (lists[2] != nullptr)
			{
				Block* block = lists[2];
				lists[2] = block->prev;
				block->~Block();
				return (void*)block;
			}
			else
			{
				void* result = head;
				head = head + 96;
				return result;
			}
		}
		else if (size <= 128)
		{
			if (lists[3] != nullptr)
			{
				Block* block = lists[3];
				lists[3] = block->prev;
				block->~Block();
				return (void*)block;
			}
			else
			{
				void* result = head;
				head = head + 128;
				return result;
			}
		}
		else if (size <= 160)
		{
			if (lists[4] != nullptr)
			{
				Block* block = lists[4];
				lists[4] = block->prev;
				block->~Block();
				return (void*)block;
			}
			else
			{
				void* result = head;
				head = head + 160;
				return result;
			}
		}
		else
		{
			return malloc(size);
		}
	}

	void Allocator::Deallocate(void* pBlock, size_t size)
	{
		Block *block = (Block*)pBlock;
		::new (block) Block();
		if (size <= 32)
		{
			block->prev = lists[0];
			lists[0] = block;
		}
		else if (size <= 64)
		{
			block->prev = lists[1];
			lists[1] = block;
		}
		else if (size <= 96)
		{
			block->prev = lists[2];
			lists[2] = block;
		}
		else if (size <= 128)
		{
			block->prev = lists[3];
			lists[3] = block;
		}
		else if (size <= 160)
		{
			block->prev = lists[4];
			lists[4] = block;
		}
		else
		{
			block->~Block();
			free(pBlock);
		}
	}

}