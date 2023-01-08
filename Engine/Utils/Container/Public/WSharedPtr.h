#pragma once

namespace WEngine
{

	class RefCounterBase
	{
	public:

		RefCounterBase() = default;

		RefCounterBase(const RefCounterBase&) = delete;

		RefCounterBase& operator=(const RefCounterBase&) = delete;

		long GetCount() const { return m_refCount; }

		void operator++()
		{
#if defined(WINDOWS) || defined(_WIN32)
			InterlockedAdd(&m_refCount, 1);
#endif
		}

		void operator--()
		{
#if defined(WINDOWS) || defined(_WIN32)
			InterlockedAdd(&m_refCount, -1);
#endif
		}

		void operator++(int)
		{
#if defined(WINDOWS) || defined(_WIN32)
			InterlockedAdd(&m_refCount, 1);
#endif
		}

		void operator--(int)
		{
#if defined(WINDOWS) || defined(_WIN32)
			InterlockedAdd(&m_refCount, -1);
#endif
		}

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	private:

		long volatile m_refCount = 0;
		
	};

	template<typename T>
	class WSharedPtr
	{
	public:

		WSharedPtr(const WSharedPtr& ptr);

		T* Get() const { return m_ptr; }

		T* operator->() { return m_ptr; }

		T& operator*() { return *m_ptr; }

		~WSharedPtr();

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

		template<typename Y, typename ...Args>
		friend WSharedPtr<Y> MakeShared(Args... args);

	private:

		WSharedPtr(T *ptr);

	private:

		T *m_ptr;

		RefCounterBase *m_counter;

	};

	template<typename T>
	inline WSharedPtr<T>::WSharedPtr(T* ptr)
		: m_ptr(ptr), m_counter(new RefCounterBase())
	{
		++(*m_counter);
	}

	template<typename T>
	inline WSharedPtr<T>::WSharedPtr(const WSharedPtr& ptr)
		: m_ptr(ptr.m_ptr), m_counter(ptr.m_counter)
	{
		++(*m_counter);
	}

	template<typename T>
	inline WSharedPtr<T>::~WSharedPtr()
	{
		--(*m_counter);
		if (m_counter->GetCount() == 0)
		{
			delete m_ptr;
			delete m_counter;
		}
	}

	template<typename T, typename ...Args>
	WSharedPtr<T> MakeShared(Args... args)
	{
		return WSharedPtr<T>(new T(args...));
	}

}