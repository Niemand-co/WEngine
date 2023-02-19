#pragma once
#include "Utils/Container/Public/WDeque.h"

namespace WEngine
{

	template<typename T>
	class WQueue
	{
	public:

		WQueue() = default;

		WQueue(const std::initializer_list<T>& list);

		WQueue(const WQueue& other);

		WQueue(WQueue&& other);

		~WQueue() = default;

		void Push(const T& element);

		T Pop();

		T& Front();

		const T& Front() const;

		T& Back();

		const T& Back() const;

		bool Empty() const;

	private:

		WDeque<T> m_queue = WDeque<T>();

	};

	template<typename T>
	inline WQueue<T>::WQueue(const std::initializer_list<T>& list)
		: m_queue(list)
	{
	}

	template<typename T>
	inline WQueue<T>::WQueue(const WQueue& other)
		: m_queue(other.m_queue)
	{
	}

	template<typename T>
	inline WQueue<T>::WQueue(WQueue&& other)
		: m_queue(WEngine::Move(other.m_queue))
	{
	}

	template<typename T>
	inline void WQueue<T>::Push(const T& element)
	{
		m_queue.AtomicPushBack(element);
	}

	template<typename T>
	inline T WQueue<T>::Pop()
	{
		return m_queue.AtomicPopFront();
	}

	template<typename T>
	inline T& WQueue<T>::Front()
	{
		return m_queue.Front();
	}

	template<typename T>
	inline const T& WQueue<T>::Front() const
	{
		return m_queue.Front();
	}

	template<typename T>
	inline T& WQueue<T>::Back()
	{
		return m_queue.Back();
	}

	template<typename T>
	inline const T& WQueue<T>::Back() const
	{
		return m_queue.Back();
	}

	template<typename T>
	inline bool WQueue<T>::Empty() const
	{
		return m_queue.Empty();
	}

}