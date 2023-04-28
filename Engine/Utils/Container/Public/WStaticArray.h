#pragma once
#include "Utils/Public/Tools.h"

namespace WEngine
{

	template<typename InElementType, uint32 NumElements, uint32 Alignment = alignof(InElementType)>
	class alignas(Alignment) WStaticArray
	{
	public:

		typedef InElementType ElementType;

		WStaticArray()
			: Storage()
		{
		}

		explicit WStaticArray(ElementType DefaultValue)
			: Storage(TMakeIntegerSequence<uint32, NumElements>(), DefaultValue)
		{
		}

		//template<typename... Args>
		//explicit WStaticArray(Args&&... args)
		//	: Storage(TMakeIntegerSequence<uint32, NumElements>(), Forward<Args>(args)...)
		//{
		//}

		WStaticArray(WStaticArray&&) = default;
		WStaticArray(const WStaticArray&) = default;
		WStaticArray& operator=(WStaticArray&&) = default;
		WStaticArray& operator=(const WStaticArray&) = default;

		FORCEINLINE InElementType& operator[](uint32 Index)
		{
			RE_ASSERT(Index < NumElements, "Out of index.");
			return Storage.Elements[Index].Element;
		}

		FORCEINLINE const InElementType& operator[](uint32 Index) const
		{
			RE_ASSERT(Index < NumElements, "Out of index.");
			return Storage.Elements[Index].Element;
		}

		friend bool operator==(const WStaticArray& A, const WStaticArray& B)
		{
			for (uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
			{
				if (!(A[ElementIndex] == B[ElementIndex]))
				{
					return false;
				}
			}
			return true;
		}

		friend bool operator!=(const WStaticArray& A, const WStaticArray& B)
		{
			for (uint32 ElementIndex = 0; ElementIndex < NumElements; ++ElementIndex)
			{
				if (!(A[ElementIndex] == B[ElementIndex]))
				{
					return true;
				}
			}
			return false;
		}

		bool IsEmpty() const
		{
			return NumElements == 0;
		}

		FORCEINLINE int32 Size() const
		{
			return NumElements;
		}

	private:

		struct alignas(Alignment) AlignedElementType
		{
			AlignedElementType() = default;

			template<typename... Args>
			explicit AlignedElementType(uint32 /* Index */, Args&&... args)
				: Element(Forward<Args>(args)...)
			{
			}

			InElementType Element;
		};

		struct ArrayStorage
		{
			ArrayStorage()
				: Elements()
			{
			}

			template<uint32... Indices, typename... Args>
			explicit ArrayStorage(TIntegerSequence<uint32, Indices...>, Args&&... args)
				: Elements{ AlignedElementType(Indices, args...)... }
			{
			}

			AlignedElementType Elements[NumElements];
		};

		ArrayStorage Storage;

	public:

		template<typename StorageElementType>
		struct RangedForIterator
		{
			explicit RangedForIterator(StorageElementType* InPtr)
				: Ptr(InPtr)
			{
			}

			auto& operator*() const
			{
				return Ptr->Element;
			}

			RangedForIterator& operator++()
			{
				++Ptr;
				return *this;
			}

			friend bool operator!=(const RangedForIterator& A, const RangedForIterator& B)
			{
				return A.Ptr != B.Ptr;
			}

		private:

			StorageElementType *Ptr;
				
		};

		using RangedForIteratorType = RangedForIterator<AlignedElementType>;
		using RangedForConstIteratorType = RangedForIterator<const AlignedElementType>;

		FORCEINLINE RangedForIteratorType      begin()       { return RangedForIteratorType(Storage.Elements); }
		FORCEINLINE RangedForConstIteratorType begin() const { return RangedForConstIteratorType(Storage.Elements); }
		FORCEINLINE RangedForIteratorType      end()       { return RangedForIteratorType(Storage.Elements + NumElements); }
		FORCEINLINE RangedForConstIteratorType end() const { return RangedForConstIteratorType(Storage.Elements + NumElements); }
	};

}