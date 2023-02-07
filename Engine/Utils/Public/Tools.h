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
	struct remove_const_and_referencet { typedef T type; };

	template<typename T>
	struct remove_const_and_referencet<const T> { typedef T type; };

	template<typename T>
	struct remove_const_and_referencet<const T&> { typedef T type; };

	template<typename T>
	struct remove_const_and_referencet<const T&&> { typedef T type; };

	template<typename T>
	struct remove_const_and_referencet<T&> { typedef T type; };

	template<typename T>
	struct remove_const_and_referencet<T&&> { typedef T type; };

	template<typename T>
	constexpr remove_reference<T>::type&& move(T&& _Val) noexcept
	{
		return static_cast<remove_reference<T>::type&&>(_Val);
	}

	template<typename T>
	void Swap(T* val1, T* val2)
	{
		T tmp;
		memcpy(&tmp, val1, sizeof(T));
		memcpy(&val1, val2, sizeof(T));
		memcpy(&val2, &tmp, sizeof(T));
	}

}