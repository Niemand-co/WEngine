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

	enum class Backend
	{
		None = 0,
		Vulkan,
	};

	class Allocator
	{
	public:

		Allocator();

		Allocator(const Allocator&) = delete;

		virtual ~Allocator();

		static void Init(Backend backend);

		static Allocator* Get();

		virtual void* Allocate(size_t size);

		virtual void Deallocate(void *pBlock, size_t size);

		virtual void* Reallocate(void *pBlock, size_t originSize, size_t newSize);

	private:

		static Allocator* g_pInstance;

	private:

		BYTE* heads[8];

		size_t sizes[8];

		Block* freeLists[8];

		Block* closedLists[8];

	};

}