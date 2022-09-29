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

		virtual void Deallocate(void* pBlock, size_t size);

	private:

		static Allocator* g_pInstance;

	private:

		BYTE* head;

		Block* lists[5];

	};

}