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
			Array = WArray(Count / 32, 0xFFFFFFFFu);
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
				Array = WArray((Count / 32) + 1, 0x00000000u);
			else
				Array = WArray(Count / 32, 0x00000000u);
		}
	}

	WBitArray::~WBitArray()
	{
	}

	void WBitArray::Init(uint32 Count, bool Value)
	{
		if (Value)
		{
			Array.Clear();
			Array.Append(WArray(Count, 0xFFFFFFFFu));
		}
		else
		{
			Array.Clear();
			Array.Append(WArray(Count, 0x00000000u));
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