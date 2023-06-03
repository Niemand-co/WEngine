#include "pch.h"
#include "Utils/Container/Public/WBitArray.h"

namespace WEngine
{

	WBitArray::WBitArray()
		: NumBits(0)
	{
	}

	WBitArray::WBitArray(const WBitArray& other)
		: Array(other.Array), NumBits(other.NumBits)
	{
	}

	WBitArray::WBitArray(uint32 Count, bool Value)
		: NumBits(Count)
	{
		if (Value)
		{
			Array = WArray<uint32>(Count / 32);
			Count = Count % 32;
			if(Count == 0)return;
			uint32 Last = 0x00000000;
			for (uint32 i = 0; i < Count; ++i)
			{
				Last |= (1 << (31 - i));
			}
			Array.Push(Last);
		}
		else
		{
			if(Count % 32 > 0)
				Array = WArray<uint32>((Count / 32) + 1);
			else
				Array = WArray<uint32>(Count / 32);
		}
	}

	WBitArray::~WBitArray()
	{
	}

	void WBitArray::Init(uint32 InNumBits, bool Value)
	{
		NumBits = InNumBits;
		if (NumBits > 0)
		{
			if (NumBits > Array.Size() * 32)
			{
				uint32 Num = (NumBits + 31) / 32;
				Array.Resize(Num);
			}
			
			if (!Value)
			{
				Memzero(Array.GetData(), Array.Size() * 32);
			}
			else
			{
				for (uint32 Index = 0; Index < Array.Size(); ++Index)
				{
					Array[Index] = 0xFFFFFFFFu;
				}
			}
		}
	}

	void WBitArray::Push(const bool NewValue)
	{
		if (NumBits % 32 == 0)
		{
			if(NewValue)
				Array.Push(1 << 31);
			else
				Array.Push(0);
		}
		if (NewValue)
		{
			uint32& Word = Array[Array.Size() - 1];
			uint32 BitIndex = NumBits % 32;
			uint32 Mask = (uint32)1 << (31 - BitIndex);
			Word |= Mask;
			NumBits++;
		}
	}

	void WBitArray::Resize(uint32 Length)
	{
		NumBits = Length;
		Array.Resize(Length / 32 + (Length % 32 == 0) ? 0 : 1);
		memset(Array.GetData(), 0, sizeof(uint32) * Array.Size());
	}

	WBitReference WBitArray::operator[](uint32 Index)
	{
		RE_ASSERT(Index < NumBits, "Out of array.");
		uint32 WordIndex = Index / 32;
		uint32& Data = Array[WordIndex];
		uint32 Mask = (uint32)1 << (31 - Index % 32);
		return WBitReference(Data, Mask);
	}

	WConstBitReference WBitArray::operator[](uint32 Index) const
	{
		RE_ASSERT(Index < NumBits, "Out of array.");
		uint32 WordIndex = Index / 32;
		const uint32& Data = Array[WordIndex];
		uint32 Mask = (uint32)1 << (31 - Index % 32);
		return WConstBitReference(Data, Mask);
	}

}