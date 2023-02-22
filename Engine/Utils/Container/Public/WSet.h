#pragma once
#include "Utils/Container/Public/WArray.h"

namespace WEngine
{

	template<typename T>
	class WSet
	{
	public:

		WSet();

		~WSet();

		bool Add(const T& element);

		bool Remove(const T& element);

		void Clear();

		WArray<T> Array();

	private:

		T *m_pData;

		size_t m_count;

		size_t m_size;

	};

	template<typename T>
	inline WSet<T>::WSet()
		: m_pData(nullptr), m_count(0), m_size(0)
	{
	}

	template<typename T>
	inline WSet<T>::~WSet()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		NormalAllocator::Get()->Deallocate(m_pData);
	}

	template<typename T>
	inline bool WSet<T>::Add(const T& element)
	{
		for (size_t index = 0; index < m_count; ++index)
		{
			if(element == m_pData[index])
				return false;
		}
		if (m_size == 0)
		{
			m_pData = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			m_pData[0] = element;
			m_count = 1;
			m_size = 1;
		}
		else if (m_count == m_size)
		{
			T *newPtr = (T*)NormalAllocator::Get()->Allocate(2 * m_size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_count);
			newPtr[m_count] = element;
			m_count++;
			m_size *= 2;
		}
		else
		{
			m_pData[m_count] = element;
			m_count++;
		}
		return true;
	}

	template<typename T>
	inline bool WSet<T>::Remove(const T& element)
	{
		for (size_t index = 0; index < m_count; ++index)
		{
			if (element == m_pData[index])
			{
				m_pData[index] = m_pData[m_count - 1];
				m_count--;
				return true;
			}
		}
		return false;
	}

	template<typename T>
	inline void WSet<T>::Clear()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		m_count = 0;
	}

	template<typename T>
	inline WArray<T> WSet<T>::Array()
	{
		return WArray<T>(m_pData, m_pData + m_count);
	}

}