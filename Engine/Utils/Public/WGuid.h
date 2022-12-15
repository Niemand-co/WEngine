#pragma once

namespace WEngine
{

	void MD5(const WString& data, unsigned int& A, unsigned int& B, unsigned int& C, unsigned int& D);

	class WGuid
	{
	public:

		WGuid(const WString& data);

		WGuid(const WGuid&) = default;

		~WGuid() = default;

		bool operator==(const WGuid& guid) const
		{
			if(A == guid.A && B == guid.B && C == guid.C && D == guid.D)
				return true;
			return false;
		}

	public:

		unsigned int A;

		unsigned int B;
		
		unsigned int C;
		
		unsigned int D;

	};

}