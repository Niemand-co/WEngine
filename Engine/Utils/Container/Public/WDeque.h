#pragma once
#include "Utils/Public/Tools.h"

namespace WEngine
{

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

		void PopFront();

		void PopBack();

		void AtomicPopFront();

		void AtomicPopBack();

		T& Front();

		const T& Front() const;

		T& Back();

		const T& Back() const;

		bool Empty() const;

	private:

		struct Node
		{
			T Val;
			class NodePtr *next = nullptr;
			class NodePtr *pre = nullptr;
		};

		struct alignas(16) NodePtr
		{
			Node *ptr;
			long long count = 0;
		};

		size_t m_capacity;

		size_t m_size;

		volatile long m_popCount;

		NodePtr* m_front;

		NodePtr* m_back;

	};

	template<typename T>
	inline WDeque<T>::WDeque()
		: m_capacity(0), m_size(0), m_front(nullptr), m_back(nullptr), m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::WDeque(const std::initializer_list<T>& list)
		: m_popCount(0)
	{
		m_capacity = m_size = list.size();
		if(m_capacity == 0)return;
		m_front = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
		m_front->ptr->Val = *list.begin();
		NodePtr *cur = m_front;
		for (unsigned int i = 1; i < list.size(); ++i)
		{
			cur->ptr->next = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
			cur->ptr->next->Val = *(list.begin() + i);
			cur = cur->ptr->next;
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
		while (m_front)
		{
			NodePtr* cur = m_front;
			m_front = m_front->ptr->next;
			cur->ptr->~Node();
			Allocator::Get()->Deallocate(cur->ptr);
			cur->~NodePtr();
			Allocator::Get()->Deallocate(cur);
		}
	}

	template<typename T>
	inline void WDeque<T>::PushFront(const T& element)
	{
		NodePtr *newFront = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
		newFront->ptr = (Node*)Allocator::Get()->Allocate(sizeof(Node));
		newFront->ptr->Val = element;
		newFront->ptr->next = m_front;
		m_front->ptr->pre = newFront;
		m_front = newFront;
	}

	template<typename T>
	inline void WDeque<T>::PushBack(const T& element)
	{
		m_back->ptr->next = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
		m_back->ptr->next->pre = m_back;
		m_back = m_back->ptr->next;
		m_back->ptr->Val = element;
	}

	template<typename T>
	inline void WDeque<T>::AtomicPushFront(const T& element)
	{
		NodePtr* newFront = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
		newFront->ptr->Val = element;
		while (true)
		{
			NodePtr *oldFront = m_front;
			newFront->ptr->next = oldFront;
			oldFront->ptr->pre = newFront;

			if(InterlockedCompareExchange128((volatile long long*)&m_front, (long long)newFront, (long long)m_popCount, (long long*)&oldFront) == oldFront)
				break;
		}
	}

	template<typename T>
	inline void WDeque<T>::AtomicPushBack(const T& element)
	{
		NodePtr*newBack = (NodePtr*)Allocator::Get()->Allocate(sizeof(NodePtr));
		newBack->ptr->Val = element;
		while (true)
		{
			NodePtr *oldBack = m_back;
			oldBack->ptr->next = newBack;
			newBack->ptr->pre = oldBack;

			if(InterlockedCompareExchange128((volatile long long*)&m_back, (long long)newBack, (long long)m_popCount, (long long*)&oldBack) == oldBack)
				break;
		}
	}

	template<typename T>
	inline void WDeque<T>::PopFront()
	{
		NodePtr* temp = m_front;
		m_front = m_front->ptr->next;
		temp->ptr->~Node();
		Allocator::Get()->Deallocate(temp->ptr);
		temp->~NodePtr();
		Allocator::Get()->Deallocate(temp);
	}

	template<typename T>
	inline void WDeque<T>::PopBack()
	{
		NodePtr* temp = m_back;
		m_back = m_back->ptr->pre;
		temp->ptr->~Node();
		Allocator::Get()->Deallocate(temp->ptr);
		temp->~NodePtr();
		Allocator::Get()->Deallocate(temp);
	}

	template<typename T>
	inline void WDeque<T>::AtomicPopFront()
	{
		while (true)
		{
			NodePtr *oldFront = m_front;
			NodePtr *newFront = m_front->ptr->next;

			if (InterlockedCompareExchange128((volatile long long*)&m_front, (long long)newFront, (long long)m_popCount, (long long*)&oldFront) == oldFront)
			{
				oldFront->ptr->~Node();
				Allocator::Get()->Deallocate(oldFront->ptr);
				oldFront->~NodePtr();
				Allocator::Get()->Deallocate(oldFront);
				InterlockedAdd(&m_popCount, 1);
				break;
			}
		}
	}

	template<typename T>
	inline void WDeque<T>::AtomicPopBack()
	{
		while (true)
		{
			NodePtr* oldBack = m_back;
			NodePtr* newBack = m_back->ptr->pre;

			if (InterlockedCompareExchange128((volatile long long*)&m_back, (long long)newBack, (long long)m_popCount, (long long*)&oldBack) == oldBack)
			{
				oldBack->ptr->~Node();
				Allocator::Get()->Deallocate(oldBack->ptr);
				oldBack->~NodePtr();
				Allocator::Get()->Deallocate(oldBack);
				InterlockedAdd(&m_popCount, 1);
				break;
			}
		}
	}

	template<typename T>
	inline T& WDeque<T>::Front()
	{
		return m_front->ptr->Val;
	}

	template<typename T>
	inline const T& WDeque<T>::Front() const
	{
		return m_front->ptr->Val;
	}

	template<typename T>
	inline T& WDeque<T>::Back()
	{
		return m_back->ptr->Val;
	}

	template<typename T>
	inline const T& WDeque<T>::Back() const
	{
		return m_back->ptr->Val;
	}

	template<typename T>
	inline bool WDeque<T>::Empty() const
	{
		return m_front == nullptr;
	}

}