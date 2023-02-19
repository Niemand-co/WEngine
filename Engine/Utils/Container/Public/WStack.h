#pragma once
#include "Utils/Container/Public/WDeque.h"

namespace WEngine
{

	template<typename T>
	class WStack
	{
	public:

		WStack() = default;

		WStack(const std::initializer_list<T>& list);

		WStack(const WStack& other);

		WStack(WStack&& other);

		~WStack() = default;

		void Push(const T& element);

		T Pop();

		T& Top();

		const T& Top() const;

		bool Empty() const;

	private:

		WDeque<T> m_queue = WDeque<T>();

	};

	template<typename T>
	inline WStack<T>::WStack(const std::initializer_list<T>& list)
		: m_queue(list)
	{
	}

	template<typename T>
	inline WStack<T>::WStack(const WStack& other)
		: m_queue(other.m_queue)
	{
	}

	template<typename T>
	inline WStack<T>::WStack(WStack&& other)
		: m_queue(WEngine::Move(other.m_queue))
	{
	}

	template<typename T>
	inline void WStack<T>::Push(const T& element)
	{
		m_queue.AtomicPushFront(element);
	}

	template<typename T>
	inline T WStack<T>::Pop()
	{
		return m_queue.AtomicPopFront();
	}

	template<typename T>
	inline T& WStack<T>::Top()
	{
		return m_queue.Front();
	}

	template<typename T>
	inline const T& WStack<T>::Top() const
	{
		return m_queue.Front();
	}

	template<typename T>
	inline bool WStack<T>::Empty() const
	{
		return m_queue.Empty();
	}

}