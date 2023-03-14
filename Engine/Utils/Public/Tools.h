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
	struct remove_const<const T*> { typedef T* type; };

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
	constexpr T&& Forward(T& _Val)
	{
		return static_cast<T&&>(_Val);
	}

	template<typename T>
	remove_const<T>::type RemoveConst(T _Val)
	{
		return const_cast<remove_const<T>::type>(_Val);
	}

	template<typename T>
	constexpr remove_reference<T>::type& Forward(typename remove_reference<T>::type& _Val)
	{
		return static_cast<remove_reference<T>::type&>(_Val);
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
		static constexpr uint8 Min() { return 0x00; }
		static constexpr uint8 Max() { return UINT8_MAX; }
	};

	template<>
	struct NumericLimits<uint16>
	{
		static constexpr uint16 Min() { return 0x0000; }
		static constexpr uint16 Max() { return UINT16_MAX; }
	};

	template<>
	struct NumericLimits<uint32>
	{
		static constexpr uint32 Min() { return 0x00000000; }
		static constexpr uint32 Max() { return UINT32_MAX; }
	};

	template<>
	struct NumericLimits<uint64>
	{
		static constexpr uint64 Min() { return 0x0000000000000000; }
		static constexpr uint64 Max() { return UINT64_MAX; }
	};

	template<>
	struct NumericLimits<int8>
	{
		static constexpr int8 Min() { return INT8_MIN; }
		static constexpr int8 Max() { return INT8_MAX; }
	};

	template<>
	struct NumericLimits<int16>
	{
		static constexpr int16 Min() { return INT16_MIN; }
		static constexpr int16 Max() { return INT16_MAX; }
	};

	template<>
	struct NumericLimits<int32>
	{
		static constexpr int32 Min() { return INT32_MIN; }
		static constexpr int32 Max() { return INT32_MAX; }
	};

	template<>
	struct NumericLimits<int64>
	{
		static constexpr int64 Min() { return INT64_MIN; }
		static constexpr int64 Max() { return INT64_MAX; }
	};

	template<typename Enum>
	constexpr bool EnumHasFlags(Enum Flags, Enum Contains)
	{
		return ((__underlying_type(Enum))Flags & (__underlying_type(Enum))Contains) != 0;
	}

	template<typename T>
	constexpr T Align(T Val, uint64 Alignment)
	{
		return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
	}

}

#define ENUM_CLASS_FLAGS(Enum) \
	inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
	inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
	inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; } \
	inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }