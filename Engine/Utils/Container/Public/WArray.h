#pragma once
#include "Utils/Public/Tools.h"
#include "Utils/Allocator/Public/Allocator.h"
#include <initializer_list>

namespace WEngine
{

	template<typename InElementyType>
	class WArray
	{
	public:

		typedef InElementyType ElementType;
		static constexpr const size_t ElementSize = sizeof(InElementyType);

		WArray()
			: ArrayNum(0),
			  Capacity(0),
			  Data(nullptr)
		{
		}

		WArray(uint32 Size)
			: ArrayNum(Size),
			  Capacity(Size)
		{
			Data = (ElementType*)GetCPUAllocator()->Allocate(Capacity * ElementSize);
			DefaultConstructItems<ElementType>(Data, ArrayNum);
		}

		template<typename OtherElementType>
		WArray(std::initializer_list<OtherElementType> list)
			: ArrayNum(list.size()),
			  Capacity(list.size())
		{
			Data = (ElementType*)GetCPUAllocator()->Allocate(Capacity * ElementSize);
			ConstructItems<ElementType>(Data, (OtherElementType*)list.begin(), ArrayNum);
		}

		WArray(std::initializer_list<ElementType> list)
			: ArrayNum(list.size()),
			Capacity(list.size())
		{
			Data = (ElementType*)GetCPUAllocator()->Allocate(Capacity * ElementSize);
			ConstructItems<ElementType>(Data, (ElementType*)list.begin(), ArrayNum);
		}

		template<typename OtherElementType>
		WArray(OtherElementType* Begin, OtherElementType* End)
			: ArrayNum(uint32(End - Begin)),
			  Capacity(uint32(End - Begin))
		{
			Data = (ElementType*)GetCPUAllocator()->Allocate(Capacity * ElementSize);
			ConstructItems<ElementType>(Data, Begin, ArrayNum);
		}

		WArray(const WArray& Other)
			: ArrayNum(Other.ArrayNum),
			  Capacity(Other.Capacity)
		{
			Data = (ElementType*)GetCPUAllocator()->Allocate(Capacity * ElementSize);
			ConstructItems<ElementType>(Data, Other.Data, ArrayNum);
		}

		explicit WArray(WArray&& Other)
			: ArrayNum(Other.ArrayNum),
			  Capacity(Other.Capacity),
			  Data(Other.Data)
		{
			Other.Data = nullptr;
			Other.ArrayNum = 0;
			Other.Capacity = 0;
		}

		~WArray()
		{
			DestructItems(this->Data, ArrayNum);
			GetCPUAllocator()->Deallocate(this->Data);
		}

		void Push(const ElementType& NewValue)
		{
			if (Capacity == ArrayNum)
			{
				Grow(Capacity);
			}

			ConstructItems<ElementType>(Data + ArrayNum, &NewValue, 1);
			ArrayNum++;
		}

		void Append(const WArray<ElementType>& Other)
		{
			ResizeForCopy(ArrayNum + Other.ArrayNum);
			Memcpy(Data + ArrayNum, Other.Data, Other.ArrayNum);
		}

		void Append(WArray<ElementType>&& Other)
		{
			ResizeForCopy(ArrayNum + Other.ArrayNum);
			Memcpy(Data + ArrayNum, Other.Data, Other.ArrayNum);
		}

		ElementType& AddInitialized()
		{
			uint32 OldNum = ArrayNum;
			uint32 NewNum = ArrayNum + 1;
			ArrayNum = NewNum;
			if (NewNum > Capacity)
			{
				Grow(Capacity);
			}
			DefaultConstructItems<ElementType>(Data + OldNum, 1);
			return *(Data + OldNum);
		}

		void AddInitialized(uint32 Num)
		{
			uint32 OldNum = ArrayNum;
			uint32 NewNum = ArrayNum + 1;
			ArrayNum = NewNum;
			if (NewNum > Capacity)
			{
				Grow(Capacity);
			}
			DefaultConstructItems<ElementType>(Data + OldNum, Num);
		}

		uint32 AddUninitialized()
		{
			uint32 OldNum = ArrayNum;
			uint32 NewNum = ArrayNum + 1;
			ArrayNum = NewNum;
			if (NewNum > Capacity)
			{
				Grow(Capacity);
			}
			return OldNum;
		}

		uint32 AddUninitialized(uint32 Num)
		{
			uint32 OldNum = ArrayNum;
			uint32 NewNum = ArrayNum + Num;
			ArrayNum = NewNum;
			if (NewNum > Capacity)
			{
				Grow(Capacity);
			}
			return OldNum;
		}

		void AddZerod(uint32 Count)
		{
			uint32 Index = AddUninitialized(Count);
			Memzero(Data + Index, ElementSize * Count);
		}

		ElementType Pop(bool bShrink = true)
		{
			--ArrayNum;
			ElementType Result = *(Data + ArrayNum);
			DestructItems(Data + ArrayNum, 1);
			return Result;
		}

		ElementType* Find(const ElementType& Val)
		{
			for (ElementType* Index = begin(); Index != end(); ++Index)
			{
				if (Val == *Index)
					return Index;
			}
			return end();
		}

		uint32 FindIndex(const ElementType& Val)
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

		void RemoveAt(uint32 Index, uint32 Num = 1, bool bShrink = true)
		{
			if (Num)
			{
				DestructItems(GetData() + Index, Num);

				uint32 NumToMove = ArrayNum - Index - Num;
				if (NumToMove)
				{
					Memcpy(Data + Index, Data + Index + Num, NumToMove * ElementSize);
				}
				ArrayNum -= Num;

				if (bShrink)
				{
					Shrink();
				}
			}
		}

		void RemoveAndSwap(uint32 Index, uint32 Num = 1, bool bShrink = true)
		{
			if (Num)
			{
				DestructItems(GetData() + Index, Num);

				uint32 NumHoles = Num;
				uint32 NumAfterHoles = ArrayNum - Index - Num;
				uint32 NumToMove = WEngine::Min(NumHoles, NumAfterHoles);
				if (NumToMove)
				{
					Memcpy(Data + Index, Data + (ArrayNum - NumToMove), NumToMove * ElementSize);
				}
				ArrayNum -= Num;

				if (bShrink)
				{
					Shrink();
				}
			}
		}

		bool RemoveSingleSwap(const ElementType& Val)
		{
			uint32 Index = FindIndex(Val);
			if (Index == ArrayNum)
			{
				return false;
			}

			RemoveAndSwap(Index);

			return true;
		}

		void SetNumZeroed(uint32 NewNum, bool bShrink = true)
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

		void Shrink()
		{
			if (Capacity > ArrayNum)
			{
				ElementType* NewPtr = (ElementType*)GetCPUAllocator()->Allocate(ArrayNum * ElementSize);
				Memcpy(NewPtr, Data, ArrayNum * ElementSize);
				GetCPUAllocator()->Deallocate(Data);
				Data = NewPtr;
				Capacity = ArrayNum;
			}
		}

		void Resize(uint32 NewNum, bool bShrink = true)
		{
			uint32 OldNum = ArrayNum;
			uint32 NewCount = NewNum - OldNum;
			ArrayNum = NewNum;
			if (NewNum > Capacity)
			{
				GrowTo(NewNum);
			}
			DefaultConstructItems<ElementType>(Data + OldNum, NewCount);
		}

		void Reserve(uint32 NewNum)
		{
			GrowTo(NewNum);
		}

		void Clear(bool bShrink = true)
		{
			DestructItems(Data, ArrayNum);
			ArrayNum = 0;
			if (bShrink)
			{
				Shrink();
			}
		}

		bool Empty()
		{
			return ArrayNum;
		}

		uint32 Size() const
		{
			return ArrayNum;
		}

		ElementType* GetData() const
		{
			return Data;
		}

		ElementType& operator[](uint32 index)
		{
			RE_ASSERT(index < ArrayNum, "Out of Index.");
			return *(Data + index);
		}

		const ElementType& operator[](uint32 index) const
		{
			RE_ASSERT(index < ArrayNum, "Out of Index.");
			return *(Data + index);
		}

		WArray<ElementType>& operator=(const WArray<ElementType>& Other)
		{
			DestructItems(Data, ArrayNum);
			ArrayNum = Other.ArrayNum;
			Capacity = Other.Capacity;
			GetCPUAllocator()->Deallocate(Data);

			Data = (ElementType*)GetCPUAllocator()->Allocate(ElementSize * Capacity);
			CopyToEmpty(Other.Data, Other.ArrayNum);
			return *this;
		}

		WArray<ElementType>& operator=(WArray<ElementType>&& Other)
		{
			ArrayNum = Other.ArrayNum;
			Capacity = Other.Capacity;
			Data = Other.Data;
			Other.ArrayNum = 0;
			Other.Capacity = 0;
			Other.Data = nullptr;
			return *this;
		}

		template<typename OtherElementType>
		WArray<ElementType>& operator=(std::initializer_list<OtherElementType> List)
		{
			ArrayNum = List.size();
			GrowTo(ArrayNum);
			ConstructItems<ElementType>(Data, List.begin(), ArrayNum);
		}

		bool operator==(const WArray<ElementType>& Other) const
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
			return GetCPUAllocator()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			GetCPUAllocator()->Deallocate(pData);
		}

	private:

		void CopyToEmpty(ElementType* Source, uint32 Count)
		{
			ArrayNum = Count;
			ConstructItems<ElementType>(Data, Source, Count);
		}

		void ResizeForCopy(uint32 NewNum)
		{
			if (NewNum > Capacity)
			{
				Grow(Capacity);
			}
			if (NewNum > ArrayNum)
			{
				DefaultConstructItems<ElementType>(Data + ArrayNum, NewNum - ArrayNum);
			}
			else if (NewNum < ArrayNum)
			{
				RemoveAt(NewNum, ArrayNum - NewNum);
			}
			ArrayNum = NewNum;
		}

		void Grow(uint32 OldSize)
		{
			if (OldSize == 0)
			{
				Data = (ElementType*)GetCPUAllocator()->Allocate(ElementSize);
				Capacity = 1;
				return;
			}
			ElementType *NewPtr = (ElementType*)GetCPUAllocator()->Allocate(2 * OldSize * ElementSize);
			Memcpy(NewPtr, Data, ArrayNum * ElementSize);
			GetCPUAllocator()->Deallocate(Data);
			Data = NewPtr;
			Capacity *= 2;
		}

		void GrowTo(uint32 NewSize)
		{
			if (NewSize <= Capacity)
			{
				return;
			}
			ElementType* NewPtr = (ElementType*)GetCPUAllocator()->Allocate(NewSize * ElementSize);
			Memcpy(NewPtr, Data, ArrayNum * ElementSize);
			GetCPUAllocator()->Deallocate(Data);
			Data = NewPtr;
			Capacity = NewSize;
		}

	private:

		uint32 ArrayNum;

		uint32 Capacity;

		ElementType *Data;

	public:

		FORCEINLINE       ElementType* begin() { return Data; }
		FORCEINLINE const ElementType* begin() const { return Data; }
		FORCEINLINE       ElementType* end() { return Data + ArrayNum; }
		FORCEINLINE const ElementType* end() const { return Data + ArrayNum; }

	};

}