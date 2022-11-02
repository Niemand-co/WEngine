#pragma once

namespace WEngine
{

	template<typename T>
	std::string GetTypeName()
	{
#if defined(_MSC_VER)
		std::string s = __FUNCSIG__;
		std::string functionName = __FUNCTION__;
		size_t start = s.find(functionName + '<') + functionName.length() + 1;
		size_t end = s.find_last_of('>');
#elif defined(__GNUC__) ||defined(__clang__)
		std::string s = __PRETTY_FUNCTION__;
#endif
		return s.substr(start, end - start);
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

	template<int Begin, int End, class Func, typename enable_if<Begin == End>::type = 0>
	void static_for(Func const& func)
	{
	}

	template<int Begin, int End, class Func, typename enable_if<Begin != End>::type = 0>
	void static_for(Func const &func)
	{
		struct Value
		{
			enum { value = Begin };
		};
		func(Value());
		static_for<Begin + 1, End>(func);
	}

	template<typename T>
	void foo(T val)
	{
		TypeList<T>();
	}

	template<typename T, typename ...Args>
	void foo(T val, Args ...args)
	{
		TypeList<T>();
		foo(args...);
	}

	template<typename T, T N>
	std::string GetIntName()
	{
#if defined(_MSC_VER)
		std::string s = __FUNCSIG__;
		std::string functionName = __FUNCTION__;
		size_t start = s.find(functionName + '<') + functionName.length() + 1;
		size_t end = s.find_last_of('>');
#elif defined(__GNUC__) ||defined(__clang__)
		std::string s = __PRETTY_FUNCTION__;
		size_t start = s.find("T = ") + 4;
		size_t end = s.find_first_of(';');
#endif
		return s.substr(start, end - start);
	}

	template<typename T, int Begin = 0, int End = 256>
	std::string GetEnumName(T N)
	{
		std::string s = "";
		static_for<Begin, End>([&](auto val)
		{
			if (N == (T)val.value)
			{
				s = GetIntName<T, T(val.value)>();
				return;
			}
		});
		size_t start = s.find("::") + 2;
		return s.substr(start);
	}

	template<typename T, int Begin = 0, int End = 256>
	T GetEnumFromName(std::string name)
	{
		for (int i = Begin; i < End; ++i)
		{
			if(name == GetEnumName((T)i));
				return (T)i;
		}
	}

	template<typename ...T>
	struct TypeList
	{
		enum { count = sizeof...(T) };
	};

	template<typename list, typename newType>
	struct PushTypeList
	{
	};

	template<typename newType, typename ...T>
	struct PushTypeList<TypeList<T...>, newType>
	{
		typedef TypeList<newType, T...> type;
	};

	template<typename _ArgsList, TypeList ...T>
	struct TemplateList
	{
	};



	template<typename ArgsList, template<typename...> class T>
	struct TemplateInstance
	{
		typedef ArgsList type;
	};

/*	template<template<typename...> class T, typename ...Args>
	struct TemplateInstance<TypeList<Args...>, T>{};*/// { typedef T<Args...> type; };

	namespace SRefl
	{

		template<typename T>
		struct BaseValue
		{
			std::string_view name;
			T value;
			enum { hasValue = true };
			
			template<typename U>
			constexpr bool operator==(const BaseValue<U>& other) const { if constexpr(is_same<T, U>::isSame)return other.value == value; else return false; }

			template<typename U>
			constexpr bool operator==(const U& v) const { if constexpr(is_same<T, U>::isSame)return v == value; else return false; }
		};

		template<>
		struct BaseValue<void>
		{
			std::string_view name;
			enum { hasValue = false };

			template<typename U>
			constexpr bool operator==(const U&) const { return false; }

			template<typename U>
			constexpr bool operator==(const BaseValue<U>&) const { return false; }
		};

		template<typename ...Es>
		struct ElementList
		{
			std::tuple<Es...> elements;
			enum { count = sizeof...(Es) };

			constexpr ElementList(Es ...elems) : elements{ elems... } {}


		};

		template<bool s, bool f>
		struct VTraitsBase
		{
			enum { isStatic = s, isFunc = f };
		};

		template<typename T>
		struct VTraits : public VTraitsBase<true, is_function<T>::isFunction>
		{

		};

		template<typename T>
		struct VTraits<T*> : public VTraitsBase<true, is_function<T>::isFunction>
		{

		};

		template<typename T, typename U>
		struct VTrats : public VTraitsBase<false, is_function<T>::isFunction>
		{

		};

		template<typename T, typename U>
		struct VTraits<T U::*> : public VTraitsBase<false, is_function<T>::isFunction>
		{
		
		};

		template<typename T>
		struct Field : public VTraits<T>, public BaseValue<T>
		{
			constexpr Field(std::string_view name, T val) : BaseValue<T>(name, val), VTraits<T>() {  }
		};

		template<typename T>
		struct Field<T*> : public VTraits<T>, public BaseValue<T>
		{
			constexpr Field(std::string_view name, T* val) : BaseValue<T>(name, val), VTraits<T>() {  }
		};

		template<typename ...Fields>
		struct FieldList : public ElementList<Fields...>
		{
			constexpr FieldList(Fields ...fields) : ElementList<Fields...>(fields...) {  }
		};

		template<typename T, typename ...ElementLists>
		struct TypeInfoBase
		{
			typedef T type;
			static constexpr ElementList list = { ElementLists{}... };
		};

		template<typename T>
		struct TypeInfo;

		template<typename ...Infos>
		struct TypeInfoList
		{

		};

	}

}