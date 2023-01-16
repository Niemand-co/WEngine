#pragma once
#include "Utils/Container/Public/WString.h"
#include "Utils/Public/Tools.h"

namespace WEngine
{

//	template<typename T>
//	WString GetTypeName()
//	{
//#if defined(_MSC_VER)
//		WString s = __FUNCSIG__;
//		WString functionName = __FUNCTION__;
//		size_t start = s.find(functionName + '<') + functionName.length() + 1;
//		size_t end = s.find_last_of('>');
//#elif defined(__GNUC__) ||defined(__clang__)
//		WString s = __PRETTY_FUNCTION__;
//#endif
//		return s.Substr(start, end - start);
//	}

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

//	template<typename T, T N>
//	WEngine::WString GetIntName()
//	{
//#if defined(_MSC_VER)
//		WEngine::WString s = __FUNCSIG__;
//		WEngine::WString functionName = __FUNCTION__;
//		size_t start = s.find(functionName + '<') + functionName.Size() + 1;
//		size_t end = s.find_last_of('>');
//#elif defined(__GNUC__) ||defined(__clang__)
//		WEngine::WString s = __PRETTY_FUNCTION__;
//		size_t start = s.find("T = ") + 4;
//		size_t end = s.find_first_of(';');
//#endif
//		return s.Substr(start, end - start);
//	}
//
//	template<typename T, int Begin = 0, int End = 256>
//	WEngine::WString GetEnumName(T N)
//	{
//		WEngine::WString s = "";
//		static_for<Begin, End>([&](auto val)
//		{
//			if (N == (T)val.value)
//			{
//				s = GetIntName<T, T(val.value)>();
//				return;
//			}
//		});
//		size_t start = s.find("::") + 2;
//		return s.Substr(start);
//	}
//
//	template<typename T, int Begin = 0, int End = 256>
//	T GetEnumFromName(WString name)
//	{
//		for (int i = Begin; i < End; ++i)
//		{
//			if(name == GetEnumName((T)i));
//				return (T)i;
//		}
//	}

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

	template<size_t N>
	struct index_sequence
	{
		enum { index = N - 1 };
		typedef index_sequence<N - 1> next_index;
	};

	template<>
	struct index_sequence<0>
	{
	};

	template<class Func>
	constexpr size_t FindIf(Func const func, index_sequence<0> index)
	{
		return -1;
	}

	template<class Func, size_t N0>
	constexpr size_t FindIf(Func const func, index_sequence<N0> sequence)
	{
		struct value { enum { val = sequence.index }; };
		return func(value()) ? sequence.index : FindIf(func, index_sequence<N0 - 1>());
	}

	template<typename ...Args>
	struct tuple;

	template<>
	struct tuple<>
	{

	};

	template<typename T, typename ...Args>
	struct tuple<T, Args...> : public tuple<Args...>
	{
		typedef T type;
		enum { count = (sizeof...(Args) + 1) };
		T element;

		constexpr tuple(T e, Args ...args) : element(e), tuple<Args...>(args...) {  }

		constexpr T get_value() const { return element; }
	};

	template<size_t N, typename ...Args>
	struct element;

	template<typename T, typename ...Args>
	struct element<0, tuple<T, Args...>>
	{
		typedef T value_type;
		typedef tuple<T, Args...> class_type;
	};

	template<size_t N>
	struct element<N, tuple<>>
	{
		static_assert(0 > N, "Out of Index.");
	};

	template<size_t N, typename T, typename ...Args>
	struct element<N, tuple<T, Args...>> : public element<N - 1, tuple<Args...>>
	{

	};

	template<size_t N, typename ...Args>
	constexpr auto get(const tuple<Args...>& t)
	{
		using type = typename element<N, tuple<Args...>>::class_type;
		using value_type = typename element<N, tuple<Args...>>::value_type;
		return value_type(((type&)t).get_value());
	}

	namespace SRefl
	{

		template<typename T>
		struct BaseValue
		{
			WString name;
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
			WString name;
			enum { hasValue = false };

			template<typename U>
			constexpr bool operator==(const U&) const { return false; }

			template<typename U>
			constexpr bool operator==(const BaseValue<U>&) const { return false; }
		};

		template<typename ...Es>
		struct ElementList
		{
			tuple<Es...> elements;
			enum { count = sizeof...(Es) };

			constexpr ElementList(Es ...elems) : elements{ elems... } {}

			constexpr size_t Find(WEngine::WString name) const
			{
				return FindIf([&](auto e) -> bool { return Get<e.val>().name == name; }, index_sequence<count>());
			}

			template<size_t N>
			constexpr auto Get() const
			{
				return get<N>(elements);
			}

			template<class Func>
			constexpr void EachMem(Func const func) const
			{
				static_for<0, count>([&](auto val){ func(Get<val.value>()); });
			}

			template<class Func>
			void EachVar(Func const func) const
			{
				static_for<0, count>([&](auto val)
					{
						if (!Get<val.value>().isFunc)
							func(Get<val.value>());
					});
			}

		};

		template<typename ...Attr>
		struct AttrList : public ElementList<Attr...>
		{
			constexpr AttrList(Attr ...attr) : ElementList<Attr...>(attr...) {  }
		};

		template<bool s, bool f>
		struct VTraitsBase { enum { isStatic = s, isFunc = f }; };

		template<typename T>
		struct VTraits : public VTraitsBase<true, is_function<T>::isFunction> { };

		template<typename T>
		struct VTraits<T*> : public VTraitsBase<true, is_function<T>::isFunction> { };

		template<typename T, typename U>
		struct VTrats : public VTraitsBase<false, is_function<T>::isFunction> { };

		template<typename T, typename U>
		struct VTraits<T U::*> : public VTraitsBase<false, is_function<T>::isFunction> { };

		template<typename T, typename AList = AttrList<>>
		struct Field : public VTraits<T>, public BaseValue<T>
		{
			AList alist;

			constexpr Field(WEngine::WString name, T val, AList as = {}) : BaseValue<T>(name, val), alist{ as } {  }
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
			typedef TypeList<ElementLists...> BaseTypes;
		};

		template<typename T>
		struct TypeInfo;

		template<typename ...Infos>
		struct TypeInfoList : public ElementList<Infos...>
		{
			constexpr TypeInfoList(Infos ...infos) : ElementList<Infos...>(infos...) {  }
		};

	}

}