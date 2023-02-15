#pragma once

namespace WEngine
{

	template<typename T>
	bool isSameTpye(T para_one, T para_two)
	{
		return true; 
	}

	template<typename T1, typename T2>
	bool isSameTpye(T1 para_one, T2 para_two)
	{
		return false;
	}

	template<typename T>
	bool isSameType(T para)
	{
		return true;
	}

	template<bool>
	struct enable_if { };

	template<>
	struct enable_if<true> { typedef int type; };

	template<>
	struct enable_if<false> { };

	template<typename T, typename U>
	struct is_same { enum { isSame = false }; };

	template<typename T>
	struct is_same<T, T> { enum { isSame = true }; };

	template<typename T>
	struct is_const { enum { isConst = false }; };

	template<typename T>
	struct is_const<const T> { enum { isConst = true }; };

	template<typename T>
	struct is_reference { enum { isRef = false }; };

	template<typename T>
	struct is_reference<T&> { enum { isRef = true }; };

	template<typename T>
	struct is_reference<T&&> { enum { isRef = true }; };

	template<typename T>
	struct is_function { enum { isFunction = !is_const<const T>::isConst && !is_reference<T>::isRef }; };

	template<typename T>
	struct remove_reference { typedef T type; };

	template<typename T>
	struct remove_reference<T&> { typedef T type; };

	template<typename T>
	struct remove_reference<T&&> { typedef T type; };

	template<typename T>
	struct remove_const { typedef T type; };

	template<typename T>
	struct remove_const<const T> { typedef T type; };

	template<typename T>
	struct remove_const_and_reference { typedef T type; };

	template<typename T>
	struct remove_const_and_reference<const T> { typedef T type; };

	template<typename T>
	struct remove_const_and_reference<const T&> { typedef T type; };

	template<typename T>
	struct remove_const_and_reference<const T&&> { typedef T type; };

	template<typename T>
	struct remove_const_and_reference<T&> { typedef T type; };

	template<typename T>
	struct remove_const_and_reference<T&&> { typedef T type; };

	template<typename T>
	constexpr remove_reference<T>::type&& Move(T&& _Val) noexcept
	{
		return static_cast<remove_reference<T>::type&&>(_Val);
	}

	template<typename T>
	constexpr remove_reference<T>::type RemoveTemp(T&& _Val) noexcept
	{
		return static_cast<remove_reference<T>::type>(_Val);
	}

	template<typename T>
	void Swap(T* val1, T* val2)
	{
		T tmp;
		memcpy(&tmp, val1, sizeof(T));
		memcpy(&val1, val2, sizeof(T));
		memcpy(&val2, &tmp, sizeof(T));
	}

	template<typename NumericType>
	struct NumericLimits
	{
	};

	template<>
	struct NumericLimits<uint8>
	{
		enum { Min = 0x00 };
		enum { Max = UINT8_MAX };
	};

	template<>
	struct NumericLimits<uint16>
	{
		enum { Min = 0x0000 };
		enum { Max = UINT16_MAX };
	};

	template<>
	struct NumericLimits<uint32>
	{
		enum { Min = 0x00000000 };
		enum { Max = UINT32_MAX };
	};

	template<>
	struct NumericLimits<uint64>
	{
		enum { Min = 0x0000000000000000 };
		enum { Max = UINT64_MAX };
	};

	template<>
	struct NumericLimits<int8>
	{
		enum { Min = INT8_MIN };
		enum { Max = INT8_MAX };
	};

	template<>
	struct NumericLimits<int16>
	{
		enum { Min = INT16_MIN };
		enum { Max = INT16_MAX };
	};

	template<>
	struct NumericLimits<int32>
	{
		enum { Min = INT32_MIN };
		enum { Max = INT32_MAX };
	};

	template<>
	struct NumericLimits<int64>
	{
		enum { Min = INT64_MIN };
		enum { Max = INT64_MAX };
	};

	template<typename Enum>
	constexpr bool EnumHasFlags(Enum Flags, Enum Contains)
	{
		return ((__underlying_type(Enum))Flags & (__underlying_type(Enum))Contains) != 0;
	}
}