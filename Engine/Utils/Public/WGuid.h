#pragma once

namespace WEngine
{

	class WString;

	void MD5Internal(byte* data, size_t byteLength, unsigned int& A, unsigned int& B, unsigned int& C, unsigned int& D);

	template<typename T>
	void MD5(T data, unsigned int& A, unsigned int& B, unsigned int& C, unsigned int& D)
	{
		MD5Internal((byte*)&data, sizeof(data), A, B, C, D);
	}

	template<typename T>
	class WGuid
	{
	public:

		WGuid(const T& data)
		{
			A = 0x67452301;
			B = 0xefcdab89;
			C = 0x98badcfe;
			D = 0x10325476;
			MD5((const remove_const_and_reference<T>::type&)data, A, B, C, D);
		}

		WGuid(const WGuid&) = default;

		~WGuid() = default;

		bool operator==(const WGuid& guid) const
		{
			if (A == guid.A && B == guid.B && C == guid.C && D == guid.D)
				return true;
			return false;
		}

		void* operator new(size_t size)
		{
			return Allocator::Get()->Allocate(size);
		}

		void operator delete(void* pData)
		{
			Allocator::Get()->Deallocate(pData);
		}

	public:

		unsigned int A;

		unsigned int B;

		unsigned int C;

		unsigned int D;

	};

}