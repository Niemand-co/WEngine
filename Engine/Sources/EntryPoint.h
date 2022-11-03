#include "Utils/Reflection/Public/Register.h"
extern WEngine::Application* WEngine::CreateApplication();

enum class Color
{
	RED = 0,
	GREEN,
	BLUE,
};

class testFather
{
public:

	int count = 10;
	
};

class test : public testFather
{
public:

	int a = 2;
	int b = 2;
	int c = 2;

	void func() { RE_LOG(__FUNCSIG__); }

	static void fun() { RE_LOG(__FUNCSIG__); }

};

std::string name = "test";

template<>
struct WEngine::SRefl::TypeInfo<testFather> : public WEngine::SRefl::TypeInfoBase<testFather>
{
	static constexpr std::string_view name = "testFather";

	static constexpr WEngine::SRefl::FieldList fields =
	{
		WEngine::SRefl::Field{"count", &testFather::count},
	};
};

template<>
struct WEngine::SRefl::TypeInfo<test> : public WEngine::SRefl::TypeInfoBase<test, testFather>
{
	static constexpr std::string_view name = "test";

	static constexpr WEngine::SRefl::FieldList fields = 
	{
		WEngine::SRefl::Field{"a", &test::a},
		WEngine::SRefl::Field{"b", &test::b},
		WEngine::SRefl::Field{"C", &test::c},
		WEngine::SRefl::Field{"func", &test::func},
		//WEngine::SRefl::Field{"func", &test::fun},
	};
};

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();
	std::cout<<WEngine::GetEnumName(Color::RED)<<std::endl;
	std::cout<<(int)WEngine::GetEnumFromName<Color>("RED")<<std::endl;
	
	constexpr size_t index = WEngine::SRefl::TypeInfo<test>::fields.Find("func");
	constexpr auto f = WEngine::SRefl::TypeInfo<test>::fields.Get<index>();
	test t;
	(t.*(f.value))();

	WEngine::SRefl::TypeInfo<test>::fields.EachMem([](auto val){if constexpr(!WEngine::is_function<decltype(val)>::isFunction)RE_LOG(val)});

	return 0;
}