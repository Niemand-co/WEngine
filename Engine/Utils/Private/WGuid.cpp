#include "pch.h"
#include "Utils/Public/WGuid.h"

namespace WEngine
{

	unsigned int K[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
						 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
						 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193,
						 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51,
						 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
						 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905,
						 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681,
						 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
						 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
						 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244,
						 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
						 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314,
						 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

	unsigned int s[] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,
						 12, 17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
						 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10,
						 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

	void shift(unsigned int& a, const unsigned int& s)
	{
		a = (a << s) | (a >> (32 - s));
	}

	unsigned int F(const unsigned int& a, const unsigned int& b, const unsigned int& c)
	{
		return (a & b) | ((~a) & c);
	}

	unsigned int G(const unsigned int& a, const unsigned int& b, const unsigned int& c)
	{
		return (a & c) | (b & (~c));
	}

	unsigned int H(const unsigned int& a, const unsigned int& b, const unsigned int& c)
	{
		return a ^ b ^ c;
	}

	unsigned int I(const unsigned int& a, const unsigned int& b, const unsigned int& c)
	{
		return b ^ (a | (~c));
	}

	void FF(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d, const unsigned int& M, const unsigned int& S, const unsigned int& t)
	{
		unsigned int tmp = a + F(b, c, d) + M + t;
		shift(tmp, S);
		a = b + tmp;
	}

	void GG(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d, const unsigned int& M, const unsigned int& S, const unsigned int& t)
	{
		unsigned int tmp = a + G(b, c, d) + M + t;
		shift(tmp, S);
		a = b + tmp;
	}

	void HH(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d, const unsigned int& M, const unsigned int& S, const unsigned int& t)
	{
		unsigned int tmp = a + H(b, c, d) + M + t;
		shift(tmp, S);
		a = b + tmp;
	}

	void II(unsigned int& a, unsigned int& b, unsigned int& c, unsigned int& d, const unsigned int& M, const unsigned int& S, const unsigned int& t)
	{
		unsigned int tmp = a + I(b, c, d) + M + t;
		shift(tmp, S);
		a = b + tmp;
	}

	void MD5(const WString& data, unsigned int& A, unsigned int& B, unsigned int& C, unsigned int& D)
	{
		size_t byteLength = data.Size();
		size_t bitLength = byteLength << 3;
		size_t rest = byteLength % 64;
		size_t addLength = rest < 56 ? 64 - rest : 128 - rest;
		size_t length = byteLength + addLength;
		byte *byteArr = (byte*)WEngine::Allocator::Get()->Allocate(length);

		size_t index = 0;
		for (; index < byteLength; ++index)
		{
			byteArr[index] = data[index];
		}

		byteArr[index++] = (byte)0x80;

		for (; index < length - 8; ++index)
		{
			byteArr[index] = 0;
		}

		for (; index < length; ++index)
		{
			byteArr[index] = (byte)(bitLength & 0xFF);
			bitLength >>= 8;
		}

		size_t num = length / 64;
		for (size_t groupIndex = 0; groupIndex < num; ++groupIndex)
		{
			unsigned int M[16];
			for (size_t i = 0; i < 16; ++i)
			{
				index = i * 4 + groupIndex * 64;
				M[i] = ((byteArr[index] & 0xFF) | (byteArr[index + 1] & 0xFF) | (byteArr[index + 2] & 0xFF) | (byteArr[index + 3] & 0xFF));
			}

			unsigned int a = A;
			unsigned int b = B;
			unsigned int c = C;
			unsigned int d = D;

			FF(a, b, c, d, M[0], 7, K[0]);
			FF(d, a, b, c, M[1], 12, K[1]);
			FF(c, d, a, b, M[2], 17, K[2]);
			FF(b, c, d, a, M[3], 22, K[3]);
			FF(a, b, c, d, M[4], 7, K[4]);
			FF(d, a, b, c, M[5], 12, K[5]);
			FF(c, d, a, b, M[6], 17, K[6]);
			FF(b, c, d, a, M[7], 22, K[7]);
			FF(a, b, c, d, M[8], 7, K[8]);
			FF(d, a, b, c, M[9], 12, K[9]);
			FF(c, d, a, b, M[10], 17, K[10]);
			FF(b, c, d, a, M[11], 22, K[11]);
			FF(a, b, c, d, M[12], 7, K[12]);
			FF(d, a, b, c, M[13], 12, K[13]);
			FF(c, d, a, b, M[14], 17, K[14]);
			FF(b, c, d, a, M[15], 22, K[15]);

			GG(a, b, c, d, M[1], 5, K[16]);
			GG(d, a, b, c, M[6], 8, K[17]);
			GG(c, d, a, b, M[11], 14, K[18]);
			GG(b, c, d, a, M[0], 20, K[19]);
			GG(a, b, c, d, M[5], 5, K[20]);
			GG(d, a, b, c, M[10], 8, K[21]);
			GG(c, d, a, b, M[15], 14, K[22]);
			GG(b, c, d, a, M[4], 20, K[23]);
			GG(a, b, c, d, M[9], 5, K[24]);
			GG(d, a, b, c, M[14], 8, K[25]);
			GG(c, d, a, b, M[3], 14, K[26]);
			GG(b, c, d, a, M[8], 20, K[27]);
			GG(a, b, c, d, M[13], 5, K[28]);
			GG(d, a, b, c, M[2], 8, K[29]);
			GG(c, d, a, b, M[7], 14, K[30]);
			GG(b, c, d, a, M[12], 20, K[31]);

			HH(a, b, c, d, M[5], 4, K[32]);
			HH(d, a, b, c, M[8], 11, K[33]);
			HH(c, d, a, b, M[11], 16, K[34]);
			HH(b, c, d, a, M[14], 23, K[35]);
			HH(a, b, c, d, M[1], 4, K[36]);
			HH(d, a, b, c, M[4], 11, K[37]);
			HH(c, d, a, b, M[7], 16, K[38]);
			HH(b, c, d, a, M[10], 23, K[39]);
			HH(a, b, c, d, M[13], 4, K[40]);
			HH(d, a, b, c, M[0], 11, K[41]);
			HH(c, d, a, b, M[3], 16, K[42]);
			HH(b, c, d, a, M[6], 23, K[43]);
			HH(a, b, c, d, M[9], 4, K[44]);
			HH(d, a, b, c, M[12], 11, K[45]);
			HH(c, d, a, b, M[15], 16, K[46]);
			HH(b, c, d, a, M[2], 23, K[47]);

			II(a, b, c, d, M[0], 6, K[48]);
			II(d, a, b, c, M[7], 10, K[49]);
			II(c, d, a, b, M[14], 15, K[50]);
			II(b, c, d, a, M[5], 21, K[51]);
			II(a, b, c, d, M[12], 6, K[52]);
			II(d, a, b, c, M[3], 10, K[53]);
			II(c, d, a, b, M[10], 15, K[54]);
			II(b, c, d, a, M[1], 21, K[55]);
			II(a, b, c, d, M[8], 6, K[56]);
			II(d, a, b, c, M[15], 10, K[57]);
			II(c, d, a, b, M[6], 15, K[58]);
			II(b, c, d, a, M[13], 21, K[59]);
			II(a, b, c, d, M[4], 6, K[60]);
			II(d, a, b, c, M[11], 10, K[61]);
			II(c, d, a, b, M[2], 15, K[62]);
			II(b, c, d, a, M[9], 21, K[63]);

			A += a;
			B += b;
			C += c;
			D += d;

		}

	}

	WGuid::WGuid(const WString& data)
	{
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;
		MD5(data, A, B, C, D);
	}

}