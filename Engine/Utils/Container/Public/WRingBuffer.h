#pragma once
#include "Utils/Container/Public/WArray.h"

namespace WEngine
{

	template<typename T>
	class WRingBuffer
	{
	public:

		WRingBuffer();

		WRingBuffer(std::initializer_list<T> list);

		WRingBuffer(const WRingBuffer& other);

		virtual ~WRingBuffer();

		T& operator[](size_t index) { return m_buffer[index]; }

		void Push(const T& var) { m_buffer.Push(var); }

		size_t GetCurrent() const { return m_currentIndex; }

	private:

		WArray<T> m_buffer;

		size_t m_currentIndex;

	};

}