#pragma once
#include "Utils/Public/Tools.h"
#include "HAL/Public/PlatformProcess.h"
#include "Platform/Windows/Public/WindowsPlatformProcess.h"

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

		//void AtomicPushFront(const T& element);

		void AtomicPushBack(const T& element);

		T PopFront();

		T PopBack();

		T AtomicPopFront();

		//T AtomicPopBack();

		T& Front();

		const T& Front() const;

		T& Back();

		const T& Back() const;

		bool Empty() const;

	private:

		struct Node
		{
			Node() : Val((T)0), next(nullptr) {}
			Node(T val, Node* next = nullptr) : Val(val), next(next) {}
			T Val;
			Node* next;
		};

		struct alignas(16) NodePtr
		{
			NodePtr() : ptr(nullptr), count(0) {}
			Node *ptr;
			long long count;
		};

		volatile long m_popCount;

		NodePtr m_front;

		NodePtr m_back;

	};

	template<typename T>
	inline WDeque<T>::WDeque()
		: m_popCount(0)
	{
		m_front.ptr = (Node*)QueueAllocate(sizeof(Node));
		::new (m_front.ptr) Node(0);
		m_back.ptr = m_front.ptr;
	}

	template<typename T>
	inline WDeque<T>::WDeque(const std::initializer_list<T>& list)
		: m_popCount(0)
	{
		if(list.size() == 0)return;
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
		: m_front(other.m_front),
		  m_back(other.m_back),
		  m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::WDeque(WDeque&& other)
		: m_front(WEngine::Move(other.m_front)),
		  m_back(WEngine::Move(other.m_back)),
		  m_popCount(0)
	{
	}

	template<typename T>
	inline WDeque<T>::~WDeque()
	{
		Node* ptr = m_front.ptr;
		while (ptr)
		{
			Node* tmp = ptr;
			ptr = ptr->next;
			tmp->~Node();
			QueueDeallocate(tmp);
		}
	}

	template<typename T>
	inline void WDeque<T>::PushFront(const T& element)
	{
		Node *oldFront = m_front.ptr;
		Node *newFront = (Node*)QueueAllocate(sizeof(Node));
		::new (newFront) Node(element, oldFront);
		m_front.ptr = newFront;
		if(m_back.ptr == nullptr)m_back.ptr = m_front.ptr;
	}

	template<typename T>
	inline void WDeque<T>::PushBack(const T& element)
	{
		Node *oldBack = m_back;
		Node *newBack = (Node*)QueueAllocate(sizeof(Node));
		::new (newBack) Node(element, nullptr, oldBack);
		m_back.ptr = newBack;
		if(m_front.ptr == nullptr)m_front.ptr = newBack;
	}

	template<typename T>
	inline void WDeque<T>::AtomicPushBack(const T& element)
	{
		Node *newBack = (Node*)QueueAllocate(sizeof(Node));
		::new (newBack) Node(element);
		Node* oldBack;
		while (true)
		{
			oldBack = m_back.ptr;
			Node *next = oldBack->next;
			if (oldBack != m_back.ptr)
			{
				continue;
			}

			if (next != nullptr)
			{
				PlatformProcess::CAS(&m_back.ptr, next, oldBack);
				continue;
			}

			if (PlatformProcess::CAS(&oldBack->next, newBack, next))
			{
				break;
			}
		}
		PlatformProcess::CAS(&m_back.ptr, newBack, oldBack);
	}

	template<typename T>
	inline T WDeque<T>::PopFront()
	{
		if(m_front.ptr == nullptr)return T(0);
		Node *oldFront = m_front.ptr;
		m_front.ptr = oldFront->next;
		if(m_front.ptr != nullptr)m_front.ptr->pre = nullptr;
		T tmp = oldFront->Val;
		oldFront->~Node();
		QueueDeallocate(oldFront);
		return tmp;
	}

	template<typename T>
	inline T WDeque<T>::PopBack()
	{
		if(m_back.ptr == nullptr)return T(0);
		Node *oldBack = m_back.ptr;
		m_back.ptr = oldBack->pre;
		if(m_back != nullptr)m_back.ptr->next = nullptr;
		T tmp = oldBack->Val;
		oldBack->~Node();
		QueueDeallocate(oldBack);
		return tmp;
	}

	template<typename T>
	inline T WDeque<T>::AtomicPopFront()
	{
		while (true)
		{
			NodePtr oldFront = m_front;
			Node *oldBack = m_back.ptr;
			Node *newFront = oldFront.ptr->next;

			if (oldFront.ptr != m_front.ptr)
			{
				continue;
			}

			assert(oldFront.ptr != oldBack || newFront != nullptr);

			if(oldFront.ptr == oldBack && newFront != nullptr)
			{
				PlatformProcess::CAS(&m_back.ptr, newFront, oldBack);
				continue;
			}

			if (PlatformProcess::CAS128(&m_front, m_popCount, newFront, &oldFront))
			{
				T tmp = newFront->Val;
				oldFront.ptr->~Node();
				QueueDeallocate(oldFront.ptr);
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
		return m_front.ptr == m_back.ptr;
	}

}