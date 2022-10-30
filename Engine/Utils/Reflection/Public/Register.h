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
	struct enable_if
	{

	};

	template<>
	struct enable_if<true>
	{
		typedef int type;
	};

	template<>
	struct enable_if<false>
	{

	};

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

	template<int = 0>
	std::string foo()
	{
		return __FUNCSIG__;
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

}