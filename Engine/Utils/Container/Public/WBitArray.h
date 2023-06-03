#pragma once
#include "HAL/Public/PlatformProcess.h"
#include "HAL/Public/Platform.h"
#include "Utils/Container/Public/WArray.h"

namespace WEngine
{

	class WBitReference
	{
	public:

		WBitReference(uint32& inData, uint32 inMask)
			: Data(inData), Mask(inMask)
		{
		}

		~WBitReference() = default;

		operator bool() const
		{
			return (Data & Mask) != 0;
		}

		void operator=(const bool NewValue)
		{
			if (NewValue)
			{
				Data |= Mask;
			}
			else
			{
				Data &= ~Mask;
			}
		}

		void operator|=(const bool NewValue)
		{
			if (NewValue)
			{
				Data |= Mask;
			}
		}

		void operator&=(const bool NewValue)
		{
			if (NewValue)
			{
				Data &= ~Mask;
			}
		}

		void AtomicSet(const bool NewValue)
		{
			if (NewValue)
			{
				if(Data & Mask)
					return;

				while (true)
				{
					uint32 Old = Data;
					uint32 New = Data | Mask;
					if (PlatformProcess::CAS(&Data, New, Old))
					{
						return;
					}
				}
			}
			else
			{
				if(!(Data & Mask))
					return;

				while (true)
				{
					uint32 Old = Data;
					uint32 New = Data & ~Mask;
					if (PlatformProcess::CAS(&Data, New, Old))
					{
						return;
					}
				}
			}
		}

		WBitReference& operator=(const WBitReference& other)
		{
			*this = (bool)other;
			return *this;
		}

	private:

		uint32& Data;

		uint32 Mask;

	};

	class WConstBitReference
	{
	public:

		WConstBitReference(const uint32& inData, uint32 inMask)
			: Data(inData), Mask(inMask)
		{
		}

		~WConstBitReference() = default;

		operator bool() const
		{
			return (Data & Mask) != 0;
		}

	private:

		const uint32& Data;

		uint32 Mask;

	};

	class WBitArray
	{
	public:

		WBitArray();

		WBitArray(const WBitArray& other);

		WBitArray(uint32 Count, bool Value);

		~WBitArray();

		void Init(uint32 InNumBits, bool Value);

		void Push(const bool NewValue);

		void Resize(uint32 Length);

		WBitReference operator[](uint32 Index);

		WConstBitReference operator[](uint32 Index) const;

	private:

		WArray<uint32> Array;

		uint32 NumBits;

	};

}