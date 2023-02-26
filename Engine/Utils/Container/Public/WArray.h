#pragma once
#include "Utils/Public/Tools.h"
#include <initializer_list>

namespace WEngine
{

	template<typename T>
	class WArray
	{
	public:

		WArray();

		WArray(size_t size);

		WArray(size_t size, const T& var);

		WArray(const std::initializer_list<T>& list);

		WArray(T *begin, T *end);

		WArray(const WArray& array);

		~WArray();

		void Push(T& var);

		void Push(const T& var);

		void PushForward(T& var);

		void PushForward(const T& var);

		T Pop();

		void Append(const WArray<T>& other);

		T* Find(const T& Val);

		void RemoveAndSwap(size_t index);

		//template<typename... Args>
		//void Push(Args... args);

		void Resize(size_t size);

		void Reserve(size_t size);

		void Clear();

		bool Empty();

		size_t Size() const;

		T* GetData() const;

		T* begin();

		const T* begin() const;

		T* end();

		const T* end() const;

		T& operator[](size_t index)
		{
			RE_ASSERT(index < m_size, "Out of Index.");
			return *((T*)m_pData + index);
		}

		const T& operator[](size_t index) const
		{
			RE_ASSERT(index < m_size, "Out of Index.");
			return *((T*)m_pData + index);
		}

		WArray<T>& operator=(const WArray<T>& array)
		{
			m_size = array.m_size;
			m_capasity = array.m_capasity;
			for (size_t i = 0; i < m_size; ++i)
			{
				(m_pData + i)->~T();
			}
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * m_capasity);
			memcpy(m_pData, array.m_pData, sizeof(T) * m_size);
			return *this;
		}

	private:

		size_t m_size;

		size_t m_capasity;

		T *m_pData = nullptr;

	};

	template<typename T>
	inline WArray<T>::WArray()
	{
		m_size = 0;
		m_capasity = 0;
		m_pData = nullptr;
	}

	template<typename T>
	inline WArray<T>::WArray(size_t size)
	{
		m_size = size;
		m_capasity = size;
		m_pData = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
		for (size_t i = 0; i < size; ++i)
		{
			::new (m_pData + i) T();
		}
	}

	template<typename T>
	inline WArray<T>::WArray(size_t size, const T& var)
	{
		m_size = size;
		m_capasity = size;
		if (size > 0)
		{
			m_pData = (T*)NormalAllocator::Get()->Allocate(size * sizeof(var));
		}
		for (size_t i = 0; i < size; ++i)
		{
			memcpy(m_pData + i, &var, sizeof(var));
		}
	}

	template<typename T>
	inline WArray<T>::WArray(const std::initializer_list<T>& list)
	{
		m_capasity = m_size = list.size();
		m_pData = (T*)NormalAllocator::Get()->Allocate(m_size * sizeof(T));
		memcpy(m_pData, list.begin(), m_size * sizeof(T));
	}

	template<typename T>
	inline WArray<T>::WArray(T* begin, T* end)
	{
		m_capasity = m_size = size_t(end - begin);
		m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * m_capasity);
		memcpy(m_pData, begin, sizeof(T) * m_capasity);
	}

	template<typename T>
	inline WArray<T>::WArray(const WArray& array)
	{
		m_size = array.m_size;
		m_capasity = array.m_capasity;
		m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * m_capasity);
		memcpy(m_pData, array.m_pData, sizeof(T) * m_size);
	}

	template<typename T>
	inline WArray<T>::~WArray()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		NormalAllocator::Get()->Deallocate(m_pData);
	}

	template<typename T>
	inline void WArray<T>::Push(T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}

		memcpy(m_pData + m_size, &var, sizeof(var));
		m_size++;
	}

	template<typename T>
	inline void WArray<T>::Push(const T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}
		
		::new (m_pData + m_size) T(var);
		m_size++;
	}

	template<typename T>
	inline void WArray<T>::PushForward(T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr + 1, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}
		else
		{
			for (size_t index = m_size - 1; index > 0; --index)
			{
				(m_pData)[index] = (m_pData)[index - 1];
			}
		}

		memcpy(m_pData, &var, sizeof(var));
		m_size++;
	}

	template<typename T>
	inline void WArray<T>::PushForward(const T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr + 1, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}
		else
		{
			for (size_t index = m_size - 1; index > 0; --index)
			{
				(m_pData)[index] = (m_pData)[index - 1];
			}
		}

		::new (m_pData + m_size) T(var);
		m_size++;
	}

	template<typename T>
	inline T WArray<T>::Pop()
	{
		--m_size;
		T Result = RemoveTemp(*(m_pData + m_size));
		(m_pData + m_size)->~T();
		return Result;
	}

	template<typename T>
	inline void WArray<T>::Append(const WArray<T>& other)
	{
		if (m_size + other.Size() > m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate((m_size + other.m_size) * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity = m_size + other.m_size;
		}
		memcpy(m_pData + m_size, other.m_pData, other.m_size * sizeof(T));
		m_size += other.m_size;
	}

	template<typename T>
	inline T* WArray<T>::Find(const T& Val)
	{
		for (T* Index = begin(); Index != end(); ++Index)
		{
			if(Val == *Index)
				return Index;
		}
		return end();
	}

	template<typename T>
	inline void WArray<T>::RemoveAndSwap(size_t index)
	{
		(m_pData + index)->~T();
		--m_size;
		memcpy(m_pData + index, m_pData + m_size, sizeof(T));
	}

	template<typename T>
	inline void WArray<T>::Resize(size_t size)
	{
		if (size > m_capasity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
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
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity = size;
		}
	}

	template<typename T>
	inline void WArray<T>::Clear()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		m_size = 0;
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
		if(m_size == 0)
			return nullptr;
		return m_pData;
	}

	template<typename T>
	inline T* WArray<T>::begin()
	{
		return m_pData;
	}

	template<typename T>
	inline const T* WArray<T>::begin() const
	{
		return m_pData;
	}

	template<typename T>
	inline T* WArray<T>::end()
	{
		return m_pData + m_size;
	}

	template<typename T>
	inline const T* WArray<T>::end() const
	{
		return m_pData + m_size;
	}

	//template<typename T>
	//template<typename ...Args>
	//inline void WArray<T>::Push(Args ...args)
	//{
	//	if (m_size == m_capasity)
	//	{
	//		void* newPtr = NormalAllocator::Get()->Allocate(2 * m_capasity * sizeof(T));
	//		memcpy(newPtr, m_pData, sizeof(T) * m_size);
	//		NormalAllocator::Get()->Deallocate(m_pData);
	//		m_pData = newPtr;
	//		m_capasity *= 2;
	//	}

	//	::new ((T*)m_pData + m_size) T(args...);
	//	m_size++;
	//}

}