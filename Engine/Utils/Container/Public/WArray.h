#pragma once

namespace WEngine
{

	template<typename T>
	class WArray
	{
	public:

		WArray(size_t size = 0);

		WArray(size_t size, const T& var);

		WArray(const WArray&) = default;

		~WArray();

		void Push(T& var);

		void Push(const T& var);

		template<typename... Args>
		void Push(Args... args);

		void Resize(size_t size);

		void Reserve(size_t size);

		bool Empty();

		size_t Size() const;

		T* GetData() const;

		T& operator[](size_t index)
		{
			RE_ASSERT(index < m_size, "Out of Index.");
			return *((T*)m_pData + index);
		}

	private:

		size_t m_size;

		size_t m_capasity;

		void *m_pData;

	};

	template<typename T>
	inline WArray<T>::WArray(size_t size)
	{
		m_size = size;
		m_capasity = size;
		if (size > 0)
		{
			m_pData = Allocator::Get()->Allocate(size * sizeof(T));
		}
	}

	template<typename T>
	inline WArray<T>::WArray(size_t size, const T& var)
	{
		m_size = size;
		m_capasity = size;
		if (size > 0)
		{
			m_pData = Allocator::Get()->Allocate(size * sizeof(var));
		}
		for (size_t i = 0; i < size; ++i)
		{
			memcpy(((T*)m_pData) + i, &var, sizeof(var));
		}
	}

	template<typename T>
	inline WArray<T>::~WArray()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			((T*)m_pData + i)->~T();
		}
		WEngine::Allocator::Get()->Deallocate(m_pData);
	}

	template<typename T>
	inline void WArray<T>::Push(T& var)
	{
		if (m_size == m_capasity)
		{
			void* newPtr = WEngine::Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			WEngine::Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}

		memcpy((T*)m_pData + m_size, &var, sizeof(var));
		m_size++;
	}

	template<typename T>
	inline void WArray<T>::Push(const T& var)
	{
		if (m_size == m_capasity)
		{
			void* newPtr = WEngine::Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			WEngine::Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}
		
		::new ((T*)m_pData + m_size) T(var);
		m_size++;
	}

	template<typename T>
	inline void WArray<T>::Resize(size_t size)
	{
		if (size > m_capasity)
		{
			void *newPtr = WEngine::Allocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			WEngine::Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_size = size;
			m_capasity = size;
		}
		else
		{
			m_size = size;
		}
	}

	template<typename T>
	inline void WArray<T>::Reserve(size_t size)
	{
		if (size > m_capasity)
		{
			void* newPtr = WEngine::Allocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			WEngine::Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity = size;
		}
	}

	template<typename T>
	inline bool WArray<T>::Empty()
	{
		return m_size == 0;
	}

	template<typename T>
	inline size_t WArray<T>::Size() const
	{
		return m_size;
	}

	template<typename T>
	inline T* WArray<T>::GetData() const
	{
		return (T*)m_pData;
	}

	template<typename T>
	template<typename ...Args>
	inline void WArray<T>::Push(Args ...args)
	{
		if (m_size == m_capasity)
		{
			void* newPtr = WEngine::Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			WEngine::Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}

		::new ((T*)m_pData + m_size) T(args...);
		m_size++;
	}

}