#include "Utils/Reflection/Public/Register.h"
extern WEngine::Application* WEngine::CreateApplication();

enum class Color
{
	RED = 0,
	GREEN,
	BLUE,
};

class test
{
public:

	int a;
	int b;
	int c;

	void func() {}

};

std::string name = "test";

template<>
struct WEngine::SRefl::TypeInfo<test> : public WEngine::SRefl::TypeInfoBase<test>
{
	static constexpr std::string_view name = "test";

	static constexpr WEngine::SRefl::FieldList fields = 
	{
		WEngine::SRefl::Field{"a", &test::a},
		WEngine::SRefl::Field{"b", &test::b},
		WEngine::SRefl::Field{"C", &test::c},
		WEngine::SRefl::Field{"func", &test::func},
	};
};

int main(int argc, char** argv)
{
	//WEngine::Application* g_pApp = WEngine::CreateApplication();
	//g_pApp->Tick();
	std::cout<<WEngine::GetEnumName(Color::RED)<<std::endl;
	std::cout<<(int)WEngine::GetEnumFromName<Color>("RED")<<std::endl;
	
	return 0;
}