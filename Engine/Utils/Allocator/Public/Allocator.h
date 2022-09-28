#pragma once

#define MIN_BLOCK_SIZE

namespace WEngine
{

	struct Block
	{
		Block() = default;
		~Block() = default;
		Block* prev;
	};

	class Allocator
	{
	public:

		Allocator();

		Allocator(const Allocator&) = delete;

		~Allocator();

		static Allocator* Get();

		void* Allocate(size_t size);

		void Deallocate(void* pBlock);

	private:

		void AllocateList(size_t size)
		{

		}

	private:

		static Allocator* g_pInstance;

	private:

		BYTE* head;

		Block* lists[5];

	};

}