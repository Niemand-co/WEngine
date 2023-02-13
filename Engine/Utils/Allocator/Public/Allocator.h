#pragma once
#include "Utils/Container/Public/WQueue.h"

#define MIN_BLOCK_SIZE

namespace WEngine
{

	enum class Backend
	{
		None = 0,
		Vulkan,
	};

	BYTE GetIndex(size_t blockSize);
	size_t GetBlockSize(BYTE index);

	template<uint32 NumBlock>
	class Allocator
	{
	public:

		Allocator();

		Allocator(const Allocator&) = delete;

		virtual ~Allocator();

		static void Init(Backend backend);

		static Allocator* Get();

		virtual void* Allocate(size_t size);

		virtual void Deallocate(void *pBlock);

		virtual void* Reallocate(void *pBlock, size_t newSize);

	private:

		static Allocator* g_pInstance;

	private:

		WQueue<void*> freeLists[NumBlock];

	};

	template<uint32 NumBlock>
	Allocator<NumBlock>::Allocator()
	{
		for (unsigned int i = 0; i < NumBlock; ++i)
		{
			::new (&freeLists[i]) WQueue<void*>();
		}
	}

	template<uint32 NumBlock>
	Allocator<NumBlock>::~Allocator()
	{
	}

	template<uint32 NumBlock>
	void Allocator<NumBlock>::Init(Backend backend)
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

	template<uint32 NumBlock>
	Allocator<NumBlock>* Allocator<NumBlock>::Get()
	{
		return g_pInstance;
	}

	template<uint32 NumBlock>
	void* Allocator<NumBlock>::Allocate(size_t size)
	{
		if (size <= 0)
		{
			return nullptr;
		}
		else if (size > GetBlockSize(NumBlock))
		{
			BYTE* block = (BYTE*)malloc(size + sizeof(BYTE));
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
				BYTE* data = (BYTE*)malloc(blockCount * blockSize + blockCount * sizeof(BYTE));
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

	template<uint32 NumBlock>
	void Allocator<NumBlock>::Deallocate(void* pBlock)
	{
		if (pBlock == nullptr)
			return;
		BYTE index = *((BYTE*)pBlock - 1);
		if (index == 0)
		{
			free((void*)((BYTE*)pBlock - 1));
		}
		else
		{
			index -= 1;
			freeLists[index].Push(pBlock);
		}
	}

	template<uint32 NumBlock>
	void* Allocator<NumBlock>::Reallocate(void* pBlock, size_t newSize)
	{
		BYTE index = *((BYTE*)pBlock - 1);
		if (index == 0)
		{
			BYTE* newBlock = (BYTE*)realloc((BYTE*)pBlock - 1, newSize + sizeof(BYTE));
			return newBlock + 1;
		}
		if (newSize <= 4096 && index - 1 == GetIndex(newSize))
			return pBlock;
		BYTE* newBlock = (BYTE*)Allocate(newSize);
		std::memcpy(newBlock, pBlock, GetBlockSize(index - 1));
		Deallocate(pBlock);
		return newBlock;
	}


}
typedef WEngine::Allocator<8> NormalAllocator;