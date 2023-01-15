#pragma once
#include <queue>

namespace WEngine
{

	template<typename T>
	class WQueue
	{
	public:

		WQueue();

		WQueue(const std::initializer_list<T>& list);

		WQueue(const WQueue& other);

		WQueue(WQueue&& other);

		~WQueue();

		void Push(const T& element);

		void AtomicPush(const T& element);

		T Pop();


		T& Front();

		const T& Front() const;

		T& Back();

		const T& Back() const;

		bool Empty() const;

	private:

		size_t m_capacity;

		size_t m_size;

		size_t m_front;

		size_t m_back;

		T *m_pData;

	};

	template<typename T>
	inline WQueue<T>::WQueue()
		: m_pData(nullptr), m_capacity(0), m_size(0), m_front(0), m_back(0)
	{
	}

	template<typename T>
	inline WQueue<T>::WQueue(const std::initializer_list<T>& list)
		: m_front(0)
	{
		m_capacity = m_size = list.size();
		m_pData = (T*)Allocator::Get()->Allocate(m_size * sizeof(T));
		m_back = m_size - 1;
		memcpy(m_pData, list.begin(), m_size * sizeof(T));
	}

	template<typename T>
	inline WQueue<T>::WQueue(const WQueue& other)
		: m_size(other.m_size),
		  m_capacity(other.m_capacity),
		  m_front(other.m_front),
		  m_back(other.m_back)
	{
		m_pData = (T*)Allocator::Get()->Allocate(m_size * sizeof(T));
		memcpy(m_pData, other.m_pData, m_size * sizeof(T));
	}

	template<typename T>
	inline WQueue<T>::WQueue(WQueue&& other)
		: m_size(WEngine::move(other.m_size)),
		  m_capacity(WEngine::move(other.m_capacity)),
		  m_front(WEngine::move(other.m_front)),
		  m_back(WEngine::move(other.m_back))
	{
	}

	template<typename T>
	inline WQueue<T>::~WQueue()
	{
		Allocator::Get()->Deallocate(m_pData);
	}

	template<typename T>
	inline void WQueue<T>::Push(const T& element)
	{
		if (m_capacity == 0)
		{
			m_pData = (T*)Allocator::Get()->Allocate(sizeof(T));
			m_pData[0] = element;
			m_capacity = m_size = 1;
		}
	}

	template<typename T>
	inline void WQueue<T>::AtomicPush(const T& element)
	{
	}

	template<typename T>
	inline T WQueue<T>::Pop()
	{
	}

	template<typename T>
	inline T& WQueue<T>::Front()
	{
		// TODO: 在此处插入 return 语句
	}

	template<typename T>
	inline const T& WQueue<T>::Front() const
	{
		// TODO: 在此处插入 return 语句
	}

	template<typename T>
	inline T& WQueue<T>::Back()
	{
		// TODO: 在此处插入 return 语句
	}

	template<typename T>
	inline const T& WQueue<T>::Back() const
	{
		// TODO: 在此处插入 return 语句
	}

	template<typename T>
	inline bool WQueue<T>::Empty() const
	{
		return false;
	}

}