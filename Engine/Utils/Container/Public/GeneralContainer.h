#pragma once
#include <initializer_list>

namespace WEngine
{

	template<typename T>
	class GeneralSequence
	{
	public:

		GeneralSequence();

		GeneralSequence(const GeneralSequence& other);

		GeneralSequence(GeneralSequence&& other);

		GeneralSequence(const std::initializer_list<T>& list);

		~GeneralSequence();

		void Push(T& var);

		void Push(const T& var);

		void PushForward(T& var);

		void PushForward(const T& var);

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
		
	private:

		T* m_pData;

		size_t m_size;

		size_t m_capacity;

	};

	template<typename T>
	GeneralSequence<T>::GeneralSequence()
	{

	}

	template<typename T>
	inline GeneralSequence<T>::GeneralSequence(const GeneralSequence& other)
		: m_size(other.m_size),
		  m_capacity(other.m_capacity),
		  m_pData((T*)Allocator::Get()->Allocate(other.m_size * sizeof(T)))
	{
		memcpy(m_pData, other.m_pData, m_size * sizeof(T));
	}

	template<typename T>
	inline GeneralSequence<T>::GeneralSequence(GeneralSequence&& other)
		: m_size(WEngine::move(other.m_size)),
		  m_capacity(WEngine::move(other.m_capacity)),
		  m_pData((WEngine::move(other.m_pData))
	{
		std::queue
	}

	template<typename T>
	inline GeneralSequence<T>::GeneralSequence(const std::initializer_list<T>& list)
	{
	}

	template<typename T>
	inline GeneralSequence<T>::~GeneralSequence()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		Allocator::Get()->Deallocate(m_pData);
	}

	template<typename T>
	inline void GeneralSequence<T>::Push(T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)Allocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}

		memcpy(m_pData + m_size, &var, sizeof(var));
		m_size++;
	}

	template<typename T>
	inline void GeneralSequence<T>::Push(const T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)Allocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			NormalAllocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity *= 2;
		}

		::new (m_pData + m_size) T(var);
		m_size++;
	}

	template<typename T>
	inline void GeneralSequence<T>::PushForward(T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)Allocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr + 1, m_pData, sizeof(T) * m_size);
			Allocator::Get()->Deallocate(m_pData);
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
	inline void GeneralSequence<T>::PushForward(const T& var)
	{
		if (m_capasity == 0)
		{
			m_capasity = 1;
			m_pData = (T*)Allocator::Get()->Allocate(sizeof(T));
		}
		else if (m_size == m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(2 * m_capasity * sizeof(T));
			memcpy(newPtr + 1, m_pData, sizeof(T) * m_size);
			Allocator::Get()->Deallocate(m_pData);
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
	inline void GeneralSequence<T>::Resize(size_t size)
	{
		if (size > m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			Allocator::Get()->Deallocate(m_pData);
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
	inline void GeneralSequence<T>::Reserve(size_t size)
	{
		if (size > m_capasity)
		{
			T* newPtr = (T*)Allocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, m_pData, sizeof(T) * m_size);
			Allocator::Get()->Deallocate(m_pData);
			m_pData = newPtr;
			m_capasity = size;
		}
	}

	template<typename T>
	inline void GeneralSequence<T>::Clear()
	{
		for (size_t i = 0; i < m_size; ++i)
		{
			(m_pData + i)->~T();
		}
		m_size = 0;
	}

	template<typename T>
	inline bool GeneralSequence<T>::Empty()
	{
		return m_size == 0;
	}

	template<typename T>
	inline size_t GeneralSequence<T>::Size() const
	{
		return m_size;
	}

	template<typename T>
	inline T* GeneralSequence<T>::GetData() const
	{
		return m_pData;
	}

	template<typename T>
	inline T* GeneralSequence<T>::begin()
	{
		return m_pData;
	}

	template<typename T>
	inline const T* GeneralSequence<T>::begin() const
	{
		return m_pData;
	}

	template<typename T>
	inline T* GeneralSequence<T>::end()
	{
		return m_pData + m_size;
	}

	template<typename T>
	inline const T* GeneralSequence<T>::end() const
	{
		return m_pData + m_size;
	}

}