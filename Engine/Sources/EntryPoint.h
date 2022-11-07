#include "Utils/Reflection/Public/Register.h"
extern WEngine::Application* WEngine::CreateApplication();

class test
{
public:
	int a;
};

template<>
struct WEngine::SRefl::TypeInfo<test> : public WEngine::SRefl::TypeInfoBase<test>
{
	static constexpr std::string_view name = "test";

	static constexpr FieldList fields = 
	{
		Field{"a", &test::a, AttrList{}},
	};
};

int main(int argc, char** argv)
{
	WEngine::Application* g_pApp = WEngine::CreateApplication();
	g_pApp->Tick();

	//test t;
	//t.a = 10;

	//WEngine::SRefl::TypeInfo<test>::fields.EachMem([&t](auto e)
	//{
	//	if (!e.isFunc)
	//	{
	//		RE_LOG(t.*(e.value));
	//	}
	//});

	return 0;
}