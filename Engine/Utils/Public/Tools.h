#pragma once
#include "Utils/Reflection/Public/Register.h"

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

	template<typename T>
	constexpr remove_reference<T>::type&& move(T&& _Val) noexcept
	{
		return static_cast<remove_reference<T>::type&&>(_Val);
	}

}