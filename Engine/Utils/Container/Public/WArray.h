#pragma once
#include "Utils/Public/Tools.h"
#include "Utils/Allocator/Public/Allocator.h"
#include <initializer_list>

namespace WEngine
{

	template<typename T>
	class WArray
	{
	public:

		WArray();

		WArray(size_t size);

		WArray(size_t size, const T& var);

		WArray(const std::initializer_list<T>& list);

		WArray(T *begin, T *end);

		WArray(const WArray& array);

		~WArray();

		void Push(T& var);

		void Push(const T& var);

		void PushForward(T& var);

		void PushForward(const T& var);

		T& AddInitialized();

		void AddInitialized(uint32 Num);

		void AddZerod(uint32 Count);

		T Pop();

		void Append(const WArray<T>& other);

		T* Find(const T& Val);

		uint32 FindIndex(const T& Val);

		void RemoveAt(uint32 Index, uint32 Num = 1, bool bShrink = true);

		void RemoveAndSwap(uint32 Index, uint32 Num = 1, bool bShrink = true);

		bool RemoveSingleSwap(const T& Val);

		void SetNumZeroed(uint32 NewNum, bool bShrink = true);

		void Shrink();

		//template<typename... Args>
		//void Push(Args... args);

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

		T& operator[](size_t index)
		{
			RE_ASSERT(index < ArrayNum, "Out of Index.");
			return *((T*)Data + index);
		}

		const T& operator[](size_t index) const
		{
			RE_ASSERT(index < ArrayNum, "Out of Index.");
			return *((T*)Data + index);
		}

		WArray<T>& operator=(const WArray<T>& array)
		{
			for (size_t i = 0; i < ArrayNum; ++i)
			{
				if(Data + i != nullptr)
					(Data + i)->~T();
			}
			ArrayNum = array.ArrayNum;
			Capacity = array.Capacity;
			NormalAllocator::Get()->Deallocate(Data);
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * Capacity);
			memset(Data, 0, sizeof(T) * Capacity);
			for (uint32 i = 0; i < ArrayNum; ++i)
			{
				Data[i] = array[i];
			}
			return *this;
		}

		bool operator==(const WArray<T>& Other) const
		{
			if (ArrayNum != Other.ArrayNum)
			{
				return false;
			}

			for (uint32 Index = 0; Index < ArrayNum; ++Index)
			{
				if (Data[Index] != Other[Index])
				{
					return false;
				}
			}

			return true;
		}

		void* operator new(size_t size)
		{
			return NormalAllocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			NormalAllocator::Get()->Deallocate(pData);
		}

	private:

		size_t ArrayNum;

		size_t Capacity;

		T *Data = nullptr;

	};

	template<typename T>
	inline WArray<T>::WArray()
	{
		ArrayNum = 0;
		Capacity = 0;
		Data = nullptr;
	}

	template<typename T>
	inline WArray<T>::WArray(size_t size)
	{
		ArrayNum = size;
		Capacity = size;
		Data = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
		for (size_t i = 0; i < size; ++i)
		{
			::new (Data + i) T();
		}
	}

	template<typename T>
	inline WArray<T>::WArray(size_t size, const T& var)
	{
		ArrayNum = size;
		Capacity = size;
		if (size > 0)
		{
			Data = (T*)NormalAllocator::Get()->Allocate(size * sizeof(var));
		}
		for (size_t i = 0; i < size; ++i)
		{
			memcpy(Data + i, &var, sizeof(var));
		}
	}

	template<typename T>
	inline WArray<T>::WArray(const std::initializer_list<T>& list)
	{
		Capacity = ArrayNum = list.size();
		Data = (T*)NormalAllocator::Get()->Allocate(ArrayNum * sizeof(T));
		memcpy(Data, list.begin(), ArrayNum * sizeof(T));
	}

	template<typename T>
	inline WArray<T>::WArray(T* begin, T* end)
	{
		Capacity = ArrayNum = size_t(end - begin);
		Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * Capacity);
		memcpy(Data, begin, sizeof(T) * Capacity);
	}

	template<typename T>
	inline WArray<T>::WArray(const WArray& array)
	{
		ArrayNum = array.ArrayNum;
		Capacity = array.Capacity;
		Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T) * Capacity);
		memcpy(Data, array.Data, sizeof(T) * ArrayNum);
	}

	template<typename T>
	inline WArray<T>::~WArray()
	{
		for (size_t i = 0; i < ArrayNum; ++i)
		{
			if(Data + i != nullptr)
				(Data + i)->~T();
		}
		NormalAllocator::Get()->Deallocate(Data);
	}

	template<typename T>
	inline void WArray<T>::Push(T& var)
	{
		if (Capacity == 0)
		{
			Capacity = 1;
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			memset(Data, 0, sizeof(T));
		}
		else if (ArrayNum == Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity *= 2;
			memset(Data + ArrayNum, 0, sizeof(T) * (Capacity - ArrayNum));
		}

		*(Data + ArrayNum) = var;
		ArrayNum++;
	}

	template<typename T>
	inline void WArray<T>::Push(const T& var)
	{
		if (Capacity == 0)
		{
			Capacity = 1;
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			memset(Data, 0, sizeof(T));
		}
		else if (ArrayNum == Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity *= 2;
			memset(Data + ArrayNum, 0, sizeof(T) * (Capacity - ArrayNum));
		}
		
		*(Data + ArrayNum) = var;
		ArrayNum++;
	}

	template<typename T>
	inline void WArray<T>::PushForward(T& var)
	{
		if (Capacity == 0)
		{
			Capacity = 1;
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			memset(Data, 0, sizeof(T));
		}
		else if (ArrayNum == Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
			memcpy(newPtr + 1, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity *= 2;
			memset(Data, 0, sizeof(T));
		}
		else
		{
			for (size_t index = ArrayNum - 1; index > 0; --index)
			{
				(Data)[index] = (Data)[index - 1];
			}
		}

		*Data = var;
		ArrayNum++;
	}

	template<typename T>
	inline void WArray<T>::PushForward(const T& var)
	{
		if (Capacity == 0)
		{
			Capacity = 1;
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			memset(Data, 0, sizeof(T));
		}
		else if (ArrayNum == Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
			memcpy(newPtr + 1, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity *= 2;
			memset(Data, 0, sizeof(T));
		}
		else
		{
			for (size_t index = ArrayNum - 1; index > 0; --index)
			{
				(Data)[index] = (Data)[index - 1];
			}
		}

		*Data = var;
		ArrayNum++;
	}

	template<typename T>
	inline T& WArray<T>::AddInitialized()
	{
		if (Capacity == 0)
		{
			Capacity = 1;
			Data = (T*)NormalAllocator::Get()->Allocate(sizeof(T));
			memset(Data, 0, sizeof(T));
		}
		else if (ArrayNum == Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity *= 2;
			memset(Data + ArrayNum, 0, sizeof(T) * (Capacity - ArrayNum));
		}

		::new (Data + ArrayNum) T();
		ArrayNum++;
		return *(Data + ArrayNum - 1);
	}

	template<typename T>
	inline void WArray<T>::AddInitialized(uint32 Num)
	{
		Resize(ArrayNum + Num);
	}

	template<typename T>
	inline void WArray<T>::AddZerod(uint32 Count)
	{
		Resize(ArrayNum + Count);
		WEngine::Memzero(Data + ArrayNum, sizeof(T) * Count);
	}

	template<typename T>
	inline T WArray<T>::Pop()
	{
		--ArrayNum;
		T Result = RemoveTemp(*(Data + ArrayNum));
		(Data + ArrayNum)->~T();
		return Result;
	}

	template<typename T>
	inline void WArray<T>::Append(const WArray<T>& other)
	{
		if (ArrayNum + other.Size() > Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate((ArrayNum + other.ArrayNum) * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity = ArrayNum + other.ArrayNum;
		}
		memcpy(Data + ArrayNum, other.Data, other.ArrayNum * sizeof(T));
		ArrayNum += other.ArrayNum;
	}

	template<typename T>
	inline T* WArray<T>::Find(const T& Val)
	{
		for (T* Index = begin(); Index != end(); ++Index)
		{
			if(Val == *Index)
				return Index;
		}
		return end();
	}

	template<typename T>
	inline uint32 WArray<T>::FindIndex(const T& Val)
	{
		for (uint32 Index = 0; Index < ArrayNum; ++Index)
		{
			if (Val == Data[Index])
			{
				return Index;
			}
		}
		return ArrayNum;
	}

	template<typename T>
	inline void WArray<T>::RemoveAt(uint32 Index, uint32 Num /* = 1 */, bool bShrink /* = false */)
	{
		if (Num)
		{
			DestructItems(GetData() + Index, Num);

			uint32 NumToMove = ArrayNum - Index - Num;
			if (NumToMove)
			{
				memcpy(Data + Index, Data + Index + Num, NumToMove * sizeof(T));
			}
			ArrayNum -= Num;

			if (bShrink)
			{
				Shrink();
			}
		}
	}

	template<typename T>
	inline void WArray<T>::RemoveAndSwap(uint32 Index, uint32 Num /* = 1 */, bool bShrink /* = false */)
	{
		if (Num)
		{
			DestructItems(GetData() + Index, Num);

			uint32 NumHoles = Num;
			uint32 NumAfterHoles = ArrayNum - Index - Num;
			uint32 NumToMove = WEngine::Min(NumHoles, NumAfterHoles);
			if (NumToMove)
			{
				memcpy(Data + Index, Data + (ArrayNum - NumToMove), NumToMove * sizeof(T));
			}
			ArrayNum -= Num;

			if (bShrink)
			{
				Shrink();
			}
		}
	}

	template<typename T>
	inline bool WArray<T>::RemoveSingleSwap(const T& Val)
	{
		uint32 Index = FindIndex(Val);
		if (Index == ArrayNum)
		{
			return false;
		}

		RemoveAndSwap(Index);

		return true;
	}

	template<typename T>
	inline void WArray<T>::SetNumZeroed(uint32 NewNum, bool bShrink /* = true */)
	{
		if (NewNum > ArrayNum)
		{
			AddZerod(NewNum - ArrayNum);
		}
		else if (NewNum < 0)
		{
			RE_ERROR("Undefined process.");
		}
		else if (NewNum < ArrayNum)
		{
			RemoveAt(NewNum, ArrayNum - NewNum, bShrink);
		}
	}

	template<typename T>
	inline void WArray<T>::Shrink()
	{
		if (Capacity > ArrayNum)
		{
			T* NewPtr = (T*)NormalAllocator::Get()->Allocate(ArrayNum * sizeof(T));
			memcpy(NewPtr, Data, ArrayNum * sizeof(T));
			NormalAllocator::Get()->Deallocate(Data);
			Data = NewPtr;
			Capacity = ArrayNum;
		}
	}

	template<typename T>
	inline void WArray<T>::Resize(size_t size)
	{
		if (size > Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity = size;
			for (uint32 Offset = ArrayNum; Offset < Capacity; ++Offset)
			{
				::new (Data + Offset) T();
			}
			ArrayNum = size;
		}
		else
		{
			ArrayNum = size;
		}
	}

	template<typename T>
	inline void WArray<T>::Reserve(size_t size)
	{
		if (size > Capacity)
		{
			T* newPtr = (T*)NormalAllocator::Get()->Allocate(size * sizeof(T));
			memcpy(newPtr, Data, sizeof(T) * ArrayNum);
			NormalAllocator::Get()->Deallocate(Data);
			Data = newPtr;
			Capacity = size;
			memset(Data + ArrayNum, 0, sizeof(T) * (Capacity - ArrayNum));
		}
	}

	template<typename T>
	inline void WArray<T>::Clear()
	{
		for (size_t i = 0; i < ArrayNum; ++i)
		{
			(Data + i)->~T();
		}
		memset(Data, 0, sizeof(T) * ArrayNum);
		ArrayNum = 0;
	}

	template<typename T>
	inline bool WArray<T>::Empty()
	{
		return ArrayNum == 0;
	}

	template<typename T>
	inline size_t WArray<T>::Size() const
	{
		return ArrayNum;
	}

	template<typename T>
	inline T* WArray<T>::GetData() const
	{
		return Data;
	}

	template<typename T>
	inline T* WArray<T>::begin()
	{
		return Data;
	}

	template<typename T>
	inline const T* WArray<T>::begin() const
	{
		return Data;
	}

	template<typename T>
	inline T* WArray<T>::end()
	{
		return Data + ArrayNum;
	}

	template<typename T>
	inline const T* WArray<T>::end() const
	{
		return Data + ArrayNum;
	}

	//template<typename T>
	//template<typename ...Args>
	//inline void WArray<T>::Push(Args ...args)
	//{
	//	if (ArrayNum == Capacity)
	//	{
	//		void* newPtr = NormalAllocator::Get()->Allocate(2 * Capacity * sizeof(T));
	//		memcpy(newPtr, Data, sizeof(T) * ArrayNum);
	//		NormalAllocator::Get()->Deallocate(Data);
	//		Data = newPtr;
	//		Capacity *= 2;
	//	}

	//	::new ((T*)Data + ArrayNum) T(args...);
	//	ArrayNum++;
	//}

}