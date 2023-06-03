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
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

	private:

		long volatile m_refCount = 0;
		
	};

	template<typename T>
	class WSharedPtr
	{
	public:

		WSharedPtr() = default;

		WSharedPtr(const WSharedPtr& Other);

		explicit WSharedPtr(WSharedPtr&& Other);

		WSharedPtr(T *ptr);

		T* Get() const { return Ptr; }

		RefCounterBase* GetCounter() const { return Counter; }

		T* operator->() { return Ptr; }

		const T* operator->() const { return Ptr; }

		T& operator*() { return *Ptr; }

		WSharedPtr<T>& operator=(const WSharedPtr<T>& Other)
		{
			if(Ptr == Other.Ptr)
				return *this;
			if (Ptr != nullptr)
			{
				--(*Counter);
				if (Counter->GetCount() == 0)
				{
					delete Ptr;
					delete Counter;
				}
			}
			Ptr = Other.Ptr;
			Counter = Other.Counter;
			++(*Counter);
			return *this;
		}

		WSharedPtr<T>& operator=(WSharedPtr<T>&& Other)
		{
			Ptr = Other.Ptr;
			Counter = Other.Counter;
			Other.Counter = nullptr;
			Other.Ptr = nullptr;
			return *this;
		}

		void operator=(T* ptr)
		{
			if(Ptr == ptr)
				return;
			if (Ptr != nullptr)
			{
				--(*Counter);
				if (Counter->GetCount() == 0)
				{
					delete Ptr;
					delete Counter;
				}
			}
			Ptr = ptr;
			Counter = new RefCounterBase();
			++(*Counter);
		}

		bool operator==(const WSharedPtr<T>& other)
		{
			return Ptr == other.Ptr;
		}

		bool operator==(T* ptr)
		{
			return Ptr == ptr;
		}

		bool operator!=(const WSharedPtr<T>& other)
		{
			return !(Ptr == other.Ptr);
		}

		bool operator!=(T* ptr)
		{
			return !(Ptr == ptr);
		}

		virtual ~WSharedPtr();

		void* operator new(size_t size)
		{
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

		template<typename Y, typename ...Args>
		friend WSharedPtr<Y> MakeShared(Args... args);

	private:

		T *Ptr = nullptr;

		RefCounterBase *Counter = nullptr;

	};

	template<typename T>
	inline WSharedPtr<T>::WSharedPtr(T* ptr)
		: Ptr(ptr), Counter(new RefCounterBase())
	{
		if(ptr != nullptr)
			++(*Counter);
	}

	template<typename T>
	inline WSharedPtr<T>::WSharedPtr(const WSharedPtr& Other)
		: Ptr(Other.Ptr), Counter(Other.Counter)
	{
		if(Counter)
			++(*Counter);
	}

	template<typename T>
	inline WSharedPtr<T>::WSharedPtr(WSharedPtr&& Other)
		: Ptr(Other.Ptr),
		  Counter(Other.Counter)
	{
		Other.Ptr = nullptr;
		Other.Counter = nullptr;
	}

	template<typename T>
	inline WSharedPtr<T>::~WSharedPtr()
	{
		if(Ptr == nullptr)
			return;
		--(*Counter);
		if (Counter->GetCount() == 0)
		{
			delete Ptr;
			delete Counter;
		}
	}

	template<typename T, typename ...Args>
	WSharedPtr<T> MakeShared(Args... args)
	{
		return WSharedPtr<T>(new T(args...));
	}

}