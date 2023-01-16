#pragma once
#include "Utils/Public/Tools.h"

namespace WEngine
{

	void* QueueAllocate(size_t size);

	void QueueDeallocate(void *pData);

	template<typename T>
	class WDeque
	{
	public:

		WDeque();

		WDeque(const std::initializer_list<T>& list);

		WDeque(const WDeque& other);

		WDeque(WDeque&& other);

		~WDeque();

		void PushFront(const T& element);

		void PushBack(const T& element);

		void AtomicPushFront(const T& element);

		void AtomicPushBack(const T& element);

		T PopFront();

		T PopBack();

		T AtomicPopFront();

		T AtomicPopBack();

		T& Front();

		const T& Front() const;

		T& Back();

		const T& Back() const;

		bool Empty() const;

	private:

		struct Node
		{
			Node() : Val((T)0), next(NodePtr()), pre(NodePtr()) {}
			T Val;
			class NodePtr next;
			class NodePtr pre;
		};

		struct alignas(16) NodePtr
		{
			NodePtr() : ptr(nullptr), count(0) {}
			Node *ptr;
			long long count;
		};

		size_t m_capacity;

		size_t m_size;

		volatile long m_popCount;

		NodePtr m_front;

		NodePtr m_back;

	};

	template<typename T>
	inline WDeque<T>::WDeque()
		: m_capacity(0), m_size(0), m_front(NodePtr()), m_back(NodePtr()), m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::WDeque(const std::initializer_list<T>& list)
		: m_popCount(0)
	{
		m_capacity = m_size = list.size();
		if(m_capacity == 0)return;
		m_front.ptr->Val = *list.begin();
		NodePtr cur = m_front;
		for (unsigned int i = 1; i < list.size(); ++i)
		{
			cur.ptr->next = (NodePtr*)QueueAllocate(sizeof(NodePtr));
			cur.ptr->next->Val = *(list.begin() + i);
			cur = cur.ptr->next;
			if(i == list.size() - 1)
				m_back = cur;
		}
	}

	template<typename T>
	inline WDeque<T>::WDeque(const WDeque& other)
		: m_size(other.m_size),
		  m_capacity(other.m_capacity),
		  m_front(other.m_front),
		  m_back(other.m_back),
		  m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::WDeque(WDeque&& other)
		: m_size(WEngine::move(other.m_size)),
		  m_capacity(WEngine::move(other.m_capacity)),
		  m_front(WEngine::move(other.m_front)),
		  m_back(WEngine::move(other.m_back)),
		  m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::~WDeque()
	{
		while (m_front.ptr)
		{
			NodePtr cur = m_front;
			m_front = m_front.ptr->next;
			cur.ptr->~Node();
			QueueDeallocate(cur.ptr);
		}
	}

	template<typename T>
	inline void WDeque<T>::PushFront(const T& element)
	{
		NodePtr newFront;
		newFront.ptr = (Node*)QueueAllocate(sizeof(Node));
		::new (newFront.ptr) Node();
		newFront.ptr->Val = element;
		newFront.ptr->next = m_front;
		m_front.ptr->pre = newFront;
		m_front = newFront;
	}

	template<typename T>
	inline void WDeque<T>::PushBack(const T& element)
	{
		m_back.ptr->next.ptr = (Node*)QueueAllocate(sizeof(Node));
		::new (m_back.ptr->next.ptr) Node();
		m_back.ptr->next.ptr->pre = m_back;
		m_back = m_back.ptr->next;
		m_back.ptr->Val = element;
	}

	template<typename T>
	inline void WDeque<T>::AtomicPushFront(const T& element)
	{
		NodePtr newFront;
		newFront.ptr = (Node*)QueueAllocate(sizeof(Node));
		::new (newFront.ptr) Node();
		newFront.ptr->Val = element;
		while (true)
		{
			NodePtr oldFront = m_front;
			newFront.ptr->next = oldFront;
			if (oldFront.ptr != nullptr)
			{
				oldFront.ptr->pre = newFront;
				if(InterlockedCompareExchange128((volatile long long*)&m_front, (long long)m_popCount, (long long)newFront.ptr, (long long*)&oldFront))
					break;
			}
			else
			{
				oldFront = NodePtr();
				if (InterlockedCompareExchange128((volatile long long*)&m_back, (long long)m_popCount, (long long)newFront.ptr, (long long*)&oldFront))
				{
					m_front = newFront;
					break;
				}
			}

		}
	}

	template<typename T>
	inline void WDeque<T>::AtomicPushBack(const T& element)
	{
		NodePtr newBack;
		newBack.ptr = (Node*)QueueAllocate(sizeof(Node));
		::new (newBack.ptr) Node();
		newBack.ptr->Val = element;
		while (true)
		{
			NodePtr oldBack = m_back;
			newBack.ptr->pre = oldBack;
			if (oldBack.ptr != nullptr)
			{
				oldBack.ptr->next = newBack;
				if(InterlockedCompareExchange128((volatile long long*)&m_back, (long long)m_popCount, (long long)newBack.ptr, (long long*)&oldBack))
					break;
			}
			else
			{
				oldBack = NodePtr();
				if (InterlockedCompareExchange128((volatile long long*)&m_front, (long long)m_popCount, (long long)newBack.ptr, (long long*)&oldBack))
				{
					m_back = newBack;
					break;
				}
			}
		}
	}

	template<typename T>
	inline T WDeque<T>::PopFront()
	{
		if(m_front.ptr == nullptr)return T(0);
		NodePtr temp = m_front;
		m_front = m_front.ptr->next;
		T tmp = temp.ptr->Val;
		temp.ptr->~Node();
		QueueDeallocate(temp.ptr);
		return tmp;
	}

	template<typename T>
	inline T WDeque<T>::PopBack()
	{
		if(m_back.ptr == nullptr)return T(0);
		NodePtr temp = m_back;
		m_back = m_back.ptr->pre;
		T tmp = temp.ptr->Val;
		temp.ptr->~Node();
		QueueDeallocate(temp.ptr);
		return tmp;
	}

	template<typename T>
	inline T WDeque<T>::AtomicPopFront()
	{
		if(m_front.ptr == nullptr)return T(0);
		while (true)
		{
			NodePtr oldFront = m_front;
			NodePtr newFront = m_front.ptr->next;

			if (InterlockedCompareExchange128((volatile long long*)&m_front, (long long)m_popCount, (long long)newFront.ptr, (long long*)&oldFront))
			{
				InterlockedAdd(&m_popCount, 1);
				T tmp = oldFront.ptr->Val;
				oldFront.ptr->~Node();
				QueueDeallocate(oldFront.ptr);
				return tmp;
			}
		}
	}

	template<typename T>
	inline T WDeque<T>::AtomicPopBack()
	{
		if(m_back.ptr == nullptr)return T(0);
		while (true)
		{
			NodePtr oldBack = m_back;
			NodePtr newBack = m_back.ptr->pre;

			if (InterlockedCompareExchange128((volatile long long*)&m_back, (long long)m_popCount, (long long)newBack.ptr, (long long*)&oldBack))
			{
				InterlockedAdd(&m_popCount, 1);
				T tmp = oldBack.ptr->Val;
				oldBack.ptr->~Node();
				QueueDeallocate(oldBack.ptr);
				return tmp;
			}
		}
	}

	template<typename T>
	inline T& WDeque<T>::Front()
	{
		return m_front.ptr->Val;
	}

	template<typename T>
	inline const T& WDeque<T>::Front() const
	{
		return m_front.ptr->Val;
	}

	template<typename T>
	inline T& WDeque<T>::Back()
	{
		return m_back.ptr->Val;
	}

	template<typename T>
	inline const T& WDeque<T>::Back() const
	{
		return m_back.ptr->Val;
	}

	template<typename T>
	inline bool WDeque<T>::Empty() const
	{
		return m_front.ptr == nullptr;
	}

}