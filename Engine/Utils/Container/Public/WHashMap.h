#pragma once
#include "Utils/Public/WGuid.h"

namespace WEngine
{

	template<typename T>
	class WArray;

	template<typename k, typename v>
	class WPair;

	template<typename k, typename v>
	struct Entry
	{
		size_t id;
		WPair<k, v> pair;
		Entry<k, v> *next;

		Entry(k key, const v& val, size_t hashCode) : pair(key, val), id(hashCode), next(nullptr) {  }

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}
	};

	template<typename k>
	size_t HashCode(k key)
	{
		return (size_t)key;
	}

	template<typename k, typename v, size_t (*Hash)(k key) = HashCode>
	class WHashMap
	{
	public:

		WHashMap() = default;

		~WHashMap() = default;

		bool Insert(const k& key, const v& val);

		bool Erase(const k& key);

		bool Find(const k& key);

		template<typename LAMBDA>
		void Enumerate(LAMBDA lambda)
		{
			for (Entry<k, v>* bucket : m_table)
			{
				while (bucket != nullptr)
				{
					lambda(bucket->pair);
					bucket = bucket->next;
				}
			}
		}

		v& operator[](const k& key)
		{
			size_t id = Hash(key);
			size_t index = GetIndex(id);
			Entry<k, v>* head = m_table[index];
			while (head)
			{
				if (head->id == id)
				{
					return head->pair.Second();
				}
				head = head->next;
			}

			Insert(key, {});
			return this->operator[](key);
		}

	private:

		size_t GetIndex(const size_t& index);

	private:

		Entry<k, v>* m_table[64] = {0};

	};

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline bool WHashMap<k, v, Hash>::Insert(const k& key, const v& val)
	{
		Entry<k, v> *entry = new Entry(key, val, Hash(key));
		size_t index = GetIndex(entry->id);
		Entry<k, v> *head = m_table[index];
		if (head == nullptr)
		{
			m_table[index] = entry;
			return true;
		}

		Entry<k, v> *pre = head;
		while (head)
		{
			if (head->id == entry->id)
			{
				delete entry;
				return false;
			}
			pre = head;
			head = head->next;
		}
		pre->next = entry;
		return true;
	}

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline bool WHashMap<k, v, Hash>::Erase(const k& key)
	{
		size_t id = Hash(key);
		size_t index = GetIndex(id);
		Entry<k, v>* head = m_table[index];
		if (head == nullptr)
		{
			return false;
		}

		Entry<k, v>* pre = head;
		while (head)
		{
			if (head->id == id)
			{
				if (pre == head)
				{
					m_table[index] = nullptr;
					delete head;
				}
				else
				{
					pre->next = nullptr;
					delete head;
				}
				return true;
			}
			pre = head;
			head = head->next;
		}

		return false;
	}

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline bool WHashMap<k, v, Hash>::Find(const k& key)
	{
		size_t id = Hash(key);
		size_t index = GetIndex(id);
		Entry<k, v>* head = m_table[index];
		if (head == nullptr)
		{
			return false;
		}

		while (head)
		{
			if (head->id == id)
			{
				return true;
			}
			head = head->next;
		}

		return false;
	}

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline size_t WHashMap<k, v, Hash>::GetIndex(const size_t& index)
	{
		return index & 63;
	}

}