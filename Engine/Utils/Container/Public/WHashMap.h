#pragma once
#include "Utils/Public/WGuid.h"

namespace WEngine
{

	template<typename T>
	class WArray;

	template<typename k, typename v>
	class WPair;

	template<typename v>
	struct Entry
	{
		size_t id;
		WPair<size_t, const v&> pair;
		Entry<v> *next;

		Entry(size_t key, const v& val) : id(key), pair(WEngine::MakePiar<size_t, const v&>(key, val)), next(nullptr) {  }

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
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

		WHashMap();

		~WHashMap() = default;

		bool Insert(const k& key, const v& val);

		bool Erase(const k& key);

		bool Find(const k& key);

	private:

		size_t GetIndex(const size_t& index);

	private:

		WArray<Entry<v>*> m_table;

	};

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline WHashMap<k, v, Hash>::WHashMap()
		: m_table(WArray<Entry<v>*>(64, nullptr))
	{
		
	}

	template<typename k, typename v, size_t(*Hash)(k key)>
	inline bool WHashMap<k, v, Hash>::Insert(const k& key, const v& val)
	{
		Entry<v> *entry = new Entry(Hash(key), val);
		size_t index = GetIndex(entry->id);
		Entry<v> *head = m_table[index];
		if (head == nullptr)
		{
			m_table[index] = entry;
			return true;
		}

		Entry<v> *pre = head;
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
		Entry<v>* head = m_table[index];
		if (head == nullptr)
		{
			return false;
		}

		Entry<v>* pre = head;
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
		Entry<v>* head = m_table[index];
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