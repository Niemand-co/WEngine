#pragma once

namespace WEngine
{

	class WEvent
	{
	public:

		virtual ~WEvent() = default;

		virtual void Wait() = 0;

		virtual void Reset() = 0;

		virtual void Trigger() = 0;

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	public:

		static WEvent* Create();

	};

}