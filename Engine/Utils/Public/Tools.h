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
	template<typename T, T... Indices>
	struct TIntegerSequence
	{
	};

	template <typename T, T N>
	using TMakeIntegerSequence = __make_integer_seq<TIntegerSequence, T, N>;

	template<typename ElementType>
	struct TIsElementTypePOD
	{
		enum { Value = false };
	};

	template<typename T>
	struct TIsEnum
	{
		enum { Value = __is_enum(T) };
	};

	template<typename T>
	struct TIsPointer
	{
		enum { Value = false };
	};

	template<typename T>
	struct TIsPointer<T*>
	{
		enum { Value = true };
	};

	template<typename T> struct TIsPointer<const          T> { enum { Value = TIsPointer<T>::Value }; };
	template<typename T> struct TIsPointer<      volatile T> { enum { Value = TIsPointer<T>::Value }; };
	template<typename T> struct TIsPointer<const volatile T> { enum { Value = TIsPointer<T>::Value }; };

	template<typename T>
	struct TIsArithmetic
	{
		enum { Value = false };
	};

	template<> struct TIsArithmetic<int8> { enum { Value = true }; };
	template<> struct TIsArithmetic<int16> { enum { Value = true }; };
	template<> struct TIsArithmetic<int32> { enum { Value = true }; };
	template<> struct TIsArithmetic<int64> { enum { Value = true }; };
	template<> struct TIsArithmetic<uint8> { enum { Value = true }; };
	template<> struct TIsArithmetic<uint16> { enum { Value = true }; };
	template<> struct TIsArithmetic<uint32> { enum { Value = true }; };
	template<> struct TIsArithmetic<uint64> { enum { Value = true }; };
	template<> struct TIsArithmetic<float> { enum { Value = true }; };
	template<> struct TIsArithmetic<double> { enum { Value = true }; };
	template<> struct TIsArithmetic<long double> { enum { Value = true }; };
	template<> struct TIsArithmetic<bool> { enum { Value = true }; };
	template<> struct TIsArithmetic<long> { enum { Value = true }; };
	template<> struct TIsArithmetic<unsigned long> { enum { Value = true }; };

	template<typename T> struct TIsArithmetic<const          T> { enum { Value = TIsArithmetic<T>::Value }; };
	template<typename T> struct TIsArithmetic<      volatile T> { enum { Value = TIsArithmetic<T>::Value }; };
	template<typename T> struct TIsArithmetic<const volatile T> { enum { Value = TIsArithmetic<T>::Value }; };

	template<> struct TIsElementTypePOD<int8> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<int16> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<int32> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<int64> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<uint8> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<uint16> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<uint32> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<uint64> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<float> { enum { Value = true }; };
	template<> struct TIsElementTypePOD<double> { enum { Value = true }; };

	template<typename T> struct TIsElementTypePOD<const          T> { enum { Value = TIsElementTypePOD<T>::Value }; };
	template<typename T> struct TIsElementTypePOD<      volatile T> { enum { Value = TIsElementTypePOD<T>::Value }; };
	template<typename T> struct TIsElementTypePOD<const volatile T> { enum { Value = TIsElementTypePOD<T>::Value }; };

	template <typename... Types>
	struct TOr;

	template <bool LHSValue, typename... RHS>
	struct TOrValue
	{
		static constexpr bool Value = TOr<RHS...>::value;
		static constexpr bool value = TOr<RHS...>::value;
	};

	template <typename... RHS>
	struct TOrValue<true, RHS...>
	{
		static constexpr bool Value = true;
		static constexpr bool value = true;
	};

	template <typename LHS, typename... RHS>
	struct TOr<LHS, RHS...> : TOrValue<LHS::Value, RHS...>
	{
	};

	template <>
	struct TOr<>
	{
		static constexpr bool Value = false;
		static constexpr bool value = false;
	};

	template<typename Enum>
	constexpr bool EnumHasFlags(Enum Flags, Enum Contains)
	{
		return ((__underlying_type(Enum))Flags & (__underlying_type(Enum))Contains) != 0;
	}

	template<typename T>
	FORCEINLINE constexpr T Align(T Val, uint64 Alignment)
	{
		return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
	}

	template<typename T>
	FORCEINLINE constexpr T AlignArbitrary(T Val, uint64 Alignment)
	{
		return (T)((((uint64)Val + Alignment - 1) / Alignment) * Alignment);
	}

	static FORCEINLINE void* Memzero(void* Dst, size_t Size)
	{
		return memset(Dst, 0, Size);
	}

	template<typename T>
	static FORCEINLINE void* Memzero(T& Dst)
	{
		return memset(&Dst, 0, sizeof(T));
	}

	static FORCEINLINE void MemcpyWithNoType(void* Dst, void* Src, size_t Size)
	{
		memcpy(Dst, Src, Size);
	}

	template<typename T>
	static FORCEINLINE void Memcpy(void* Dst, T* Src, size_t Size)
	{
		if(Src && Dst)
			memcpy(Dst, (void*)Src, Size);
	}

	template<typename T>
	static constexpr T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static constexpr T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	static constexpr FORCEINLINE float Clamp(const float X, const float Min, const float Max)
	{
		return X < Min ? Min : ( X > Max ? Max : X );
	}

	static constexpr FORCEINLINE double Clamp(const double X, const double Min, const double Max)
	{
		return X < Min ? Min : (X > Max ? Max : X);
	}

	template<typename T, typename R>
	struct TBitWiseConstructible
	{
		enum { Value = false };
	};

	template<typename T>
	struct TBitWiseConstructible<T, T>
	{
		enum { Value = TIsElementTypePOD<T>::Value };
	};

	template<typename T, typename R>
	struct TBitWiseConstructible<const T, R> : public TBitWiseConstructible<T, R>
	{

	};

	template<typename T>
	struct TBitWiseConstructible<const T*, T*>
	{
		enum { Value = true };
	};

	template<> struct TBitWiseConstructible<uint8, int8> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<int8, uint8> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<uint16, int16> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<int16, uint16> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<uint32, int32> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<int32, uint32> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<uint64, int64> { enum { Value = true }; };
	template<> struct TBitWiseConstructible<int64, uint64> { enum { Value = true }; };

	template<typename T>
	struct TIsZeroConstructType
	{
		enum { Value = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value };
	};

	template<typename ElementType>
	FORCEINLINE void DestructItems(ElementType* Data, uint32 Count)
	{
		while (Count > 0)
		{
			typedef ElementType DestructItemsElementTypeTypedef;

			Data->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
			++Data;
			--Count;
		}
	}

	template<typename DestinationType, typename ElementType>
	FORCEINLINE void ConstructItems(void* Dst, ElementType *Src, uint32 Count)
	{
		if constexpr (TBitWiseConstructible<DestinationType, ElementType>::Value)
		{
			Memcpy(Dst, Src, (size_t)Count * sizeof(ElementType));
		}
		else
		{
			while (Count > 0)
			{
				::new (Dst) DestinationType(*Src);
				++(DestinationType*&)Dst;
				++Src;
				--Count;
			}
		}
	}

	template<typename ElementType>
	FORCEINLINE void DefaultConstructItems(void *Address, uint32 Count)
	{
		if constexpr (TIsZeroConstructType<ElementType>::Value)
		{
			Memzero(Address, (size_t)Count * sizeof(ElementType));
		}
		else
		{
			ElementType *Element = (ElementType*)Address;
			while (Count)
			{
				::new (Element) ElementType;
				++Element;
				--Count;
			}
		}
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